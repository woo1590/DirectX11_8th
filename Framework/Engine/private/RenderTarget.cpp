#include "EnginePCH.h"
#include "RenderTarget.h"
#include "EngineCore.h"
#include "Shader.h"
#include "VIBuffer.h"

RenderTarget::RenderTarget()
{
}

RenderTarget* RenderTarget::Create(_uint width, _uint height, DXGI_FORMAT format, _float4 clearColor)
{
	RenderTarget* Instance = new RenderTarget();

	if (FAILED(Instance->Initialize(width,height,format,clearColor)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT RenderTarget::Initialize(_uint width, _uint height, DXGI_FORMAT format, _float4 clearColor)
{
	auto engine = EngineCore::GetInstance();

	m_pDevice = engine->GetDevice();
	m_pDeviceContext = engine->GetDeviceContext();

	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = format;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if(FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	return S_OK;
}

HRESULT RenderTarget::BindShaderResource(Shader* shader, const _string& constantName)
{
	return shader->BindShaderResource(constantName, m_pSRV);
}

void RenderTarget::Clear()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<_float*>(&m_ClearColor));
}

void RenderTarget::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pTexture2D);
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);
}
