#include "EnginePCH.h"
#include "GraphicDevice.h"

GraphicDevice::GraphicDevice()
{
}

GraphicDevice* GraphicDevice::Create(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY)
{
	GraphicDevice* Instance = new GraphicDevice();

	if (FAILED(Instance->Initialize(hWnd, winMode, winSizeX, winSizeY)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT GraphicDevice::Initialize(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY)
{
	_uint flag = 0;

#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;		//dx 디버그 메세지 
#endif
	D3D_FEATURE_LEVEL			featureLV;

	/* dx9 : 장치 초기화를 하기위한 설정을 쭈욱 하고나서 최종적으로 장치객체를 생성한다. */
	/* dx11 : 우선적으로 장치 객체를 생성하고 장치객체를 통해서 기타 초기화작업 및 설정을 해나간다. */

	/* 그래픽 장치를 초기화한다. */
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, nullptr, 0, D3D11_SDK_VERSION, &device, &featureLV, &deviceContext)))
		return E_FAIL;

	/* SwapChain : 더블버퍼링. 전면과 후면버퍼를 번갈아가며 화면에 보여준다.(Present) */

	/* 스왑체인객체를 생성하였고 생성한 스왑체인 객체가 백버퍼를 내장한다. 백버퍼를 생성하기 위한 ID3D11Texture2D 만든거야. */
	/* 스왑체인 객체를 만들면서 백버퍼에 해당하는 ID3D11Texture2D객체를 만들어 스왑체인 객체가 내장한다. */
	if (FAILED(InitSwapChain(hWnd, winMode, winSizeX, winSizeY)))
		return E_FAIL;

	if (FAILED(InitBackBufferRTV(winSizeX, winSizeY)))
		return E_FAIL;

	if (FAILED(InitDSV(winSizeX, winSizeY)))
		return E_FAIL;

	return S_OK;
}

void GraphicDevice::Free()
{
	__super::Free();

	Safe_Release(swapChain);
	Safe_Release(backBufferRTV);
	Safe_Release(DSV);
	Safe_Release(deviceContext);
	Safe_Release(device);
}

HRESULT GraphicDevice::ClearBackBufferView(const _float4* color)
{
	if (!deviceContext)
		return E_FAIL;

	deviceContext->ClearRenderTargetView(backBufferRTV, reinterpret_cast<const _float*>(color));

	return S_OK;
}

HRESULT GraphicDevice::ClearDepthStencilView()
{
	if (!deviceContext)
		return E_FAIL;

	deviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT GraphicDevice::Present()
{
	if (!swapChain)
		return E_FAIL;

	return swapChain->Present(0, 0);;
}

HRESULT GraphicDevice::InitSwapChain(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY)
{
	IDXGIDevice* pDevice = nullptr;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

	IDXGIAdapter* pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	/* 스왑체인을 생성한다. = 텍스쳐를 생성하는 행위 + 스왑하는 형태  */
	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	/* 백버퍼 == 텍스쳐 */
	/*텍스처(백버퍼 == ID3D11Texture2D)를 생성하는 행위*/
	SwapChain.BufferDesc.Width = winSizeX;	/* 가로 픽셀 수 */
	SwapChain.BufferDesc.Height = winSizeY;	/* 세로 픽셀 수 */

	/* float4(1.f, 1.f, 1.f, 1.f) */
	/* float4(1.f, 0.f, 0.f, 1.f) */
	SwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; /* 만든 픽셀하나의 데이터 정보 : 32BIT픽셀생성하되 부호가 없는 정규화된 수를 저장할께 */
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/* 스케치북에 사과를 그릴꺼야. */
	/* RENDER_TARGET : 그림을 당하는 대상. 스케치북 */
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;

	/*스왑하는 형태 : 모니터 주사율에 따라 조절해도 됨. */
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;

	/* 멀티샘플링 : 안티얼라이징 (계단현상방지) */
	/* 나중에 후처리 렌더링 : 멀티샘플링 지원(x) */
	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;

	SwapChain.OutputWindow = hWnd;
	SwapChain.Windowed = static_cast<BOOL>(winMode);
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	/* 백버퍼라는 텍스처(ID3D11Texture2D)를 생성했다. */
	if (FAILED(pFactory->CreateSwapChain(device, &SwapChain, &swapChain)))
		return E_FAIL;

	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT GraphicDevice::InitBackBufferRTV(_uint winSizeX, _uint winSizeY)
{
	if (!device)
		return E_FAIL;

	/* 내가 앞으로 사용 하기위한 용도의 텍스쳐를 생성하기위한 베이스 데이터를 가지고 있는 객체이다. */
	/* 내가 앞으로 사용 하기위한 용도의 텍스쳐 : ID3D11RenderTargetView, ID3D11ShaderResoureView, ID3D11DepthStencilView */
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	/* 스왑체인이 들고있던 텍스처를 가져와봐. */
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)))
		return E_FAIL;

	/* 실제 렌더타겟용도로 사용할 수 있는 텍스쳐 타입(ID3D11RenderTargetView)의 객체를 생성ㅎ나다. */
	if (FAILED(device->CreateRenderTargetView(pBackBufferTexture, nullptr, &backBufferRTV)))
		return E_FAIL;

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT GraphicDevice::InitDSV(_uint winSizeX, _uint winSizeY)
{
	if (!device)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* 깊이 버퍼의 픽셀은 백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
	/* 픽셀의 수가 다르면 아에 렌더링을 못함. */
	TextureDesc.Width = winSizeX;
	TextureDesc.Height = winSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	/* 동적? 정적?  */
	TextureDesc.Usage = D3D11_USAGE_DEFAULT /* 정적 */;
	/* 추후에 어떤 용도로 바인딩 될 수 있는 View타입의 텍스쳐를 만들기위한 Texture2D입니까? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTargetView */
	/* ShaderResourceView */
	/* DepthStencilView */

	if (FAILED(device->CreateDepthStencilView(pDepthStencilTexture, nullptr, &DSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}
