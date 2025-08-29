#include "EnginePCH.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "Renderer.h"

RenderSystem::RenderSystem()
{
}

RenderSystem* RenderSystem::Create()
{
	RenderSystem* Instance = new RenderSystem();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT RenderSystem::Initialize()
{
	m_CurrFrameProxies.resize(ENUM_CLASS(RenderGroup::Count));
	m_pRenderer = Renderer::Create(EngineCore::GetInstance()->GetDeviceContext());

	/*----Constant Buffer PerFrame----*/
	D3D11_BUFFER_DESC cbPerFrameDesc{};
	cbPerFrameDesc.ByteWidth = sizeof(CBPerFrame);
	cbPerFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerFrameDesc.MiscFlags = 0;
	cbPerFrameDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerFrameDesc, nullptr, &m_pCBPerFrame)))
		return E_FAIL;

	/*----Contant Buffer PerLight----*/
	D3D11_BUFFER_DESC cbPerlightDesc{};
	cbPerlightDesc.ByteWidth = sizeof(CBPerLight);
	cbPerlightDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerlightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerlightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerlightDesc.MiscFlags = 0;
	cbPerlightDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerlightDesc, nullptr, &m_pCBPerLight)))
		return E_FAIL;

	/*----Constant Buffer PerObject----*/
	D3D11_BUFFER_DESC cbPerObjectDesc{};
	cbPerObjectDesc.ByteWidth = sizeof(CBPerLight);
	cbPerObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerObjectDesc.MiscFlags = 0;
	cbPerObjectDesc.StructureByteStride = 0;
	
	if (FAILED(m_pDevice->CreateBuffer(&cbPerObjectDesc, nullptr, &m_pCBPerObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT RenderSystem::Render()
{
	if (FAILED(RenderNonBlend()))
		return E_FAIL;

	return S_OK;
}

void RenderSystem::Submit(std::vector<std::vector<RenderProxy>> proxies)
{
	m_CurrFrameProxies = std::move(proxies);
}

void RenderSystem::Free()
{
	__super::Free();

	for (auto& group : m_CurrFrameProxies)
		group.clear();
	m_CurrFrameProxies.clear();

}

HRESULT RenderSystem::RenderNonBlend()
{
	for (const auto& proxy : m_CurrFrameProxies[ENUM_CLASS(RenderGroup::NonBlend)])
	{
		D3D11_MAPPED_SUBRESOURCE cbData{};
		m_pDeviceContext->Map(m_pCBPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbData);
		memcpy_s(cbData.pData, sizeof(CBPerObject), &proxy.cbPerObject, sizeof(CBPerObject));
		m_pDeviceContext->Unmap(m_pCBPerObject, 0);

	}

	return S_OK;
}

void RenderSystem::Clear()
{
	for (auto& proxies : m_CurrFrameProxies)
		proxies.clear();
}
