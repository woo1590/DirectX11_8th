#include "EnginePCH.h"
#include "RenderTargetManager.h"

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

HRESULT RenderTargetManager::Intialize()
{
	return S_OK;
}

void RenderTargetManager::Free()
{
	__super::Free();
}
