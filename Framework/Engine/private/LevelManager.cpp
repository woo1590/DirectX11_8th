#include "EnginePCH.h"
#include "LevelManager.h"

LevelManager::LevelManager()
{
}

LevelManager* LevelManager::Create()
{
	LevelManager* Instance = new LevelManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LevelManager::Initialize()
{
	return S_OK;
}

void LevelManager::Free()
{
	__super::Free();
}
