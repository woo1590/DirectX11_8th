#include "EnginePCH.h"
#include "Renderer.h"
#include "EngineCore.h"
#include "VIBuffer.h"
#include "Material.h"

Renderer::Renderer()
	:m_pDevice(EngineCore::GetInstance()->GetDevice()),
	m_pDeviceContext(EngineCore::GetInstance()->GetDeviceContext())
{
	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();
}

Renderer* Renderer::Create()
{
	Renderer* Instance = new Renderer();
	
	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Renderer::Initialize()
{
	/*----Constant Buffer Per Frame----*/
	D3D11_BUFFER_DESC cbPerFrameDesc{};
	cbPerFrameDesc.ByteWidth = sizeof(CBPerFrame);
	cbPerFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerFrameDesc.MiscFlags = 0;
	cbPerFrameDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerFrameDesc, nullptr, &m_pCBPerFrame)))
		return E_FAIL;

	/*----Constant Buffer Per Object----*/
	D3D11_BUFFER_DESC cbPerObjectDesc{};
	cbPerObjectDesc.ByteWidth = sizeof(CBPerObject);
	cbPerObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerObjectDesc.MiscFlags = 0;
	cbPerObjectDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerObjectDesc, nullptr, &m_pCBPerObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT Renderer::BeginFrame()
{
	_float4x4 view, proj;
	XMStoreFloat4x4(&view, XMMatrixIdentity());
	XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), (1280.f / 720.f), 1.f, 1000.f));
	
	D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
	CBPerFrame perFrame{};
	perFrame.viewMatrix = view;
	perFrame.projMatrix = proj;
	
	m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
	memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
	m_pDeviceContext->Unmap(m_pCBPerFrame, 0);

	return S_OK;
}

HRESULT Renderer::DrawProxy(const RenderProxy& proxy)
{
	D3D11_MAPPED_SUBRESOURCE perObjectData{};
	
	m_pDeviceContext->Map(m_pCBPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &perObjectData);
	memcpy_s(perObjectData.pData, sizeof(CBPerObject), &proxy.cbPerObject, sizeof(CBPerObject));
	m_pDeviceContext->Unmap(m_pCBPerObject, 0);

	_float4x4 view, proj;
	XMStoreFloat4x4(&view, XMMatrixIdentity());
	XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), (1280.f / 720.f), 1.f, 1000.f));

	//proxy.material->SetParam("g_ViewMatrix", view);
	//proxy.material->SetParam("g_ProjMatrix", proj);
	//proxy.material->SetParam("g_WorldMatrix", proxy.cbPerObject.worldMatrix);
	
	if (FAILED(proxy.buffer->BindBuffers()))
		return E_FAIL;

	if (FAILED(proxy.material->BindMaterial(0)))
		return E_FAIL;

	return proxy.buffer->Draw();
}

HRESULT Renderer::ConnectConstantBuffer(ID3DX11Effect* pEffect)
{
	auto perFrame = pEffect->GetConstantBufferByIndex(0);
	if (FAILED(perFrame->SetConstantBuffer(m_pCBPerFrame)))
		return E_FAIL;

	auto perObject = pEffect->GetConstantBufferByIndex(2);
	if (FAILED(perObject->SetConstantBuffer(m_pCBPerObject)))
		return E_FAIL;

	return S_OK;
}

void Renderer::Free()
{
	__super::Free();

	Safe_Release(m_pCBPerFrame);
	Safe_Release(m_pCBPerObject);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
