#include "EnginePCH.h"
#include "RenderTargetManager.h"
#include "RenderTarget.h"
#include "EngineCore.h"

RenderTargetManager::RenderTargetManager()
{
}

RenderTargetManager* RenderTargetManager::Create()
{
	RenderTargetManager* Instance = new RenderTargetManager();

	if (FAILED(Instance->Intialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT RenderTargetManager::AddRenderTarget(const _string& targetTag, _uint width, _uint height, DXGI_FORMAT format, _float4 clearColor)
{
	RenderTarget* target = FindRenderTarget(targetTag);
	if (target)
		return E_FAIL;

	target = RenderTarget::Create(width, height, format, clearColor);
	if (!target)
		return E_FAIL;

	m_RenderTargets.emplace(targetTag, target);

	return S_OK;
}

HRESULT RenderTargetManager::AddMRT(const _string& mrtTag, const _string& targetTag)
{
	RenderTarget* target = FindRenderTarget(targetTag);
	if (!target)
		return E_FAIL;

	std::list<RenderTarget*>* mrt = FindMRT(mrtTag);
	if (!mrt)
	{
		std::list<RenderTarget*> targetList;
		targetList.push_back(target);

		m_MRTs.emplace(mrtTag, targetList);
	}
	else
		mrt->push_back(target);

	target->AddRef();

	return S_OK;
}

HRESULT RenderTargetManager::BindShaderResource(Shader* shader, const _string& targetTag, const _string& constantName)
{
	RenderTarget* target = FindRenderTarget(targetTag);
	if (!target)
		return E_FAIL;

	return target->BindShaderResource(shader, constantName);
}

HRESULT RenderTargetManager::BeginMRT(const _string& mrtTag)
{
	std::list<RenderTarget*>* mrt = FindMRT(mrtTag);
	if (!mrt)
	{
		MSG_BOX("MRT not exist");
		return E_FAIL;
	}

	m_pDeviceContext->OMGetRenderTargets(1, &m_pBackBuffer, &m_pDSV);

	_uint numTargets{};
	ID3D11RenderTargetView* renderTargets[8] = {};
	for (const auto& target : *mrt)
	{
		target->Clear();
		renderTargets[numTargets++] = target->GetRTV();
	}

	if (numTargets >= 8)
		return E_FAIL;

	m_pDeviceContext->OMSetRenderTargets(numTargets, renderTargets, m_pDSV);

	return S_OK;
}

HRESULT RenderTargetManager::EndMRT()
{
	ID3D11RenderTargetView* renderTargets[8] = { m_pBackBuffer };

	m_pDeviceContext->OMSetRenderTargets(8, renderTargets, m_pDSV);

	Safe_Release(m_pBackBuffer);
	Safe_Release(m_pDSV);

	return S_OK;
}

HRESULT RenderTargetManager::Intialize()
{
	auto engine = EngineCore::GetInstance();

	m_pDevice = engine->GetDevice();
	m_pDeviceContext = engine->GetDeviceContext();

	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();

	return S_OK;
}

void RenderTargetManager::Free()
{
	__super::Free();

	for (auto& pair : m_RenderTargets)
		Safe_Release(pair.second);
	m_RenderTargets.clear();

	for (auto& pair : m_MRTs)
	{
		for (auto& target : pair.second)
			Safe_Release(target);
		pair.second.clear();
	}
	m_MRTs.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

RenderTarget* RenderTargetManager::FindRenderTarget(const _string& targetTag)
{
	auto iter =  m_RenderTargets.find(targetTag);
	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

std::list<RenderTarget*>* RenderTargetManager::FindMRT(const _string& mrtTag)
{
	auto iter = m_MRTs.find(mrtTag);
	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}
