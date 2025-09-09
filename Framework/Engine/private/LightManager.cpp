#include "EnginePCH.h"
#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager* LightManager::Create()
{
	LightManager* Instance = new LightManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightManager::Initialize()
{
	return S_OK;
}

void LightManager::Free()
{
	__super::Free();
}
