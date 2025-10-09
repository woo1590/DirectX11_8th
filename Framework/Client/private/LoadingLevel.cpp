#include "pch.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "EngineCore.h"
#include "LogoLevel.h"
#include "TestLevel.h"
#include "GamePlayLevel.h"

LoadingLevel::LoadingLevel()
	:Level()
{
}

LoadingLevel* LoadingLevel::Create(LevelID nextLevelID)
{
	LoadingLevel* Instance = new LoadingLevel();

	if (FAILED(Instance->Initialize(nextLevelID)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LoadingLevel::Initialize(LevelID nextLevelID)
{
	this->m_eNextLevelID = nextLevelID;

	m_pLoader = Loader::Create(nextLevelID);
	if (!m_pLoader)
		return E_FAIL;

	return S_OK;
}

void LoadingLevel::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

void LoadingLevel::Update(_float dt)
{
	if (m_pLoader->IsFinish())
	{
		Level* nextLevel = nullptr;

		switch (m_eNextLevelID)
		{
		case Client::LevelID::Logo:
			nextLevel = LogoLevel::Create();
			break;
		case Client::LevelID::GamePlay:
			nextLevel = GamePlayLevel::Create();
			break;
		case Client::LevelID::Test:
			nextLevel = TestLevel::Create();
			break;
		default:
			break;
		}

		EngineCore::GetInstance()->ChangeLevel(ENUM_CLASS(m_eNextLevelID), nextLevel);
	}
}

HRESULT LoadingLevel::Render()
{
	m_pLoader->DebugPrint();

	return S_OK;
}
