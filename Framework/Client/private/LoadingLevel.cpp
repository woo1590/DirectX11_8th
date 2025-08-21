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
	this->nextLevelID = nextLevelID;

	loader = Loader::Create(nextLevelID);
	if (!loader)
		return E_FAIL;

	return S_OK;
}

void LoadingLevel::Free()
{
	__super::Free();

	Safe_Release(loader);
}

void LoadingLevel::Update(_float dt)
{
	if (loader->IsFinish() && GetAsyncKeyState(VK_TAB))
	{
		Level* nextLevel = nullptr;

		switch (nextLevelID)
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

		EngineCore::GetInstance()->ChangeLevel(ENUM_CLASS(nextLevelID), nextLevel);
	}
}

HRESULT LoadingLevel::Render()
{
	loader->DebugPrint();

	return S_OK;
}
