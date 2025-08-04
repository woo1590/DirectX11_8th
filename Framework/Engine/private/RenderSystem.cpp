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

void RenderSystem::Free()
{
}
