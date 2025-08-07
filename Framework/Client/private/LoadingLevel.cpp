#include "pch.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "EngineCore.h"
#include "Level.h"

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
	loader = Loader::Create(nextLevelID);
	if (!loader)
		return E_FAIL;

	return S_OK;
}

void LoadingLevel::Update(_float dt)
{
	if (loader->IsFinish())
	{
		Level* nextLevel = nullptr;

		switch (nextLevelID)
		{
		case Client::LevelID::Logo:
			break;
		case Client::LevelID::GamePlay:
			break;
		default:
			break;
		}

		EngineCore::GetInstance()->ChangeLevel(nextLevel);
	}
}

HRESULT LoadingLevel::Render()
{
	return S_OK;
}
