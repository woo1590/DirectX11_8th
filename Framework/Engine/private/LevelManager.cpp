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

	Safe_Release(m_pCurrLevel);
}

void LevelManager::Update(_float dt)
{
	if (m_pCurrLevel)
		m_pCurrLevel->Update(dt);
}

HRESULT LevelManager::Render()
{
	if (!m_pCurrLevel)
		return E_FAIL;
	else
		return m_pCurrLevel->Render();
}

void LevelManager::ChangeLevel(_uint nextLevelID, Level* nextLevel)
{
	if (m_pCurrLevel)
		EngineCore::GetInstance()->ClearResource(m_iCurrLevelID);

	Safe_Release(m_pCurrLevel);

	m_pCurrLevel = nextLevel;
	m_iCurrLevelID = nextLevelID;
}
