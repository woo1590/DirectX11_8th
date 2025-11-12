#include "EnginePCH.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "Renderer.h"
#include "DebugRenderer.h"


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
	
	m_pRenderer = Renderer::Create();
	if (!m_pRenderer)
		return E_FAIL;

	m_pDebugRenderer = DebugRenderer::Create();
	if (!m_pDebugRenderer)
		return E_FAIL;

	return S_OK;
}

HRESULT RenderSystem::RenderLoop()
{
	auto engine = EngineCore::GetInstance();

	m_pRenderer->BeginFrame();

	if (FAILED(m_pRenderer->RenderPriority(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::Priority)])))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderShadow(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::Shadow)])))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderNonBlend(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::NonBlend)])))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderLight(m_CurrFrameLights)))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderCombined()))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderBlend(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::Blend)])))
		return E_FAIL;

	if (engine->IsColliderDebugEnable())
	{
		if (FAILED(m_pDebugRenderer->ColliderDebug(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::ColliderDebug)])))
			return E_FAIL;
	}

	if (engine->IsNavDebugEnable())
	{
		m_pDebugRenderer->BeginFrame();

		if (FAILED(m_pDebugRenderer->NavMeshDebug(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::NavMeshDebug)])))
			return E_FAIL;
	}

	if (FAILED(m_pRenderer->RenderUI(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::UI)])))
		return E_FAIL;
	Clear();

	return S_OK;
}

void RenderSystem::Submit(std::vector<std::vector<RenderProxy>> proxies, std::vector<LightProxy> lights)
{
	m_CurrFrameProxies = std::move(proxies);
	m_CurrFrameLights = std::move(lights);
}

void RenderSystem::Free()
{
	__super::Free();

	Safe_Release(m_pDebugRenderer);
	Safe_Release(m_pRenderer);
	Clear();
}

void RenderSystem::Clear()
{
	for (auto& proxies : m_CurrFrameProxies)
		proxies.clear();

	m_CurrFrameLights.clear();
}
