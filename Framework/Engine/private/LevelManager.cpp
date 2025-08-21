#include "EnginePCH.h"
#include "EngineCore.h"
#include "LevelManager.h"
#include "Level.h"

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

	Safe_Release(currLevel);
}

void LevelManager::Update(_float dt)
{
	if (currLevel)
		currLevel->Update(dt);
}

HRESULT LevelManager::Render()
{
	if (!currLevel)
		return E_FAIL;
	else
		return currLevel->Render();
}

void LevelManager::ChangeLevel(_uint nextLevelID, Level* nextLevel)
{
	if (currLevel)
		EngineCore::GetInstance()->ClearResource(currLevelID);

	Safe_Release(currLevel);

	currLevel = nextLevel;
	currLevelID = nextLevelID;
}
