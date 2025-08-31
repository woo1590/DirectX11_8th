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

HRESULT RenderSystem::Render()
{
	m_pRenderer->BeginFrame();

	if (FAILED(RenderNonBlend()))
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

HRESULT RenderSystem::RenderNonBlend()
{
	for (const auto& proxy : m_CurrFrameProxies[ENUM_CLASS(RenderGroup::NonBlend)])
	{
		if (FAILED(m_pRenderer->DrawProxy(proxy)))
			return E_FAIL;
	}

	return S_OK;
}

void RenderSystem::Clear()
{
	for (auto& proxies : m_CurrFrameProxies)
		proxies.clear();
}
