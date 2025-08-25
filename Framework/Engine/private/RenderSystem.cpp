#include "EnginePCH.h"
#include "RenderSystem.h"

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
	return S_OK;
}

void RenderSystem::RenderLoop()
{
	while (m_isRunning)
	{

	}
}

void RenderSystem::StartRenderThread()
{
	m_isRunning = true;

	m_RenderThread = std::thread(&RenderSystem::RenderLoop, this);
}

void RenderSystem::StopRenderThread()
{
	m_isRunning = false;

	if (m_RenderThread.joinable())
		m_RenderThread.join();
}

void RenderSystem::Free()
{
	__super::Free();
}
