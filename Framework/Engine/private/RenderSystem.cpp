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
	
	m_pRenderer = Renderer::Create();
	if (!m_pRenderer)
		return E_FAIL;

	return S_OK;
}

HRESULT RenderSystem::RenderLoop()
{
	m_pRenderer->BeginFrame();

	if (FAILED(m_pRenderer->RenderPriority(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::Priority)])))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderNonBlend(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::NonBlend)])))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderBlend(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::Blend)])))
		return E_FAIL;

	if (FAILED(m_pRenderer->RenderUI(m_CurrFrameProxies[ENUM_CLASS(RenderGroup::UI)])))
		return E_FAIL;

	m_pRenderer->EndFrame();

	Clear();

	return S_OK;
}

void RenderSystem::Submit(std::vector<std::vector<RenderProxy>> proxies)
{
	m_CurrFrameProxies = std::move(proxies);
}

void RenderSystem::Free()
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Clear();
}

void RenderSystem::Clear()
{
	for (auto& proxies : m_CurrFrameProxies)
		proxies.clear();
}
