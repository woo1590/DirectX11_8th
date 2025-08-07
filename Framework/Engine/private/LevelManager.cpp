#include "EnginePCH.h"
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
}

void LevelManager::Update(_float dt)
{
	if (currentLevel)
		currentLevel->Update(dt);
}

HRESULT LevelManager::Render()
{
	if (!currentLevel)
		return E_FAIL;
	else
		return currentLevel->Render();
}

void LevelManager::ChangeLevel(Level* nextLevel)
{
	Safe_Release(currentLevel);

	//리소스 정리
	currentLevel = nextLevel;
}
