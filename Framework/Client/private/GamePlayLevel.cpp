#include "pch.h"
#include "GamePlayLevel.h"

GamePlayLevel::GamePlayLevel()
{
}

GamePlayLevel* GamePlayLevel::Create()
{
	GamePlayLevel* Instance = new GamePlayLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT GamePlayLevel::Initialize()
{
	return S_OK;
}

void GamePlayLevel::Free()
{
	__super::Free();
}

void GamePlayLevel::Update(_float dt)
{

}

HRESULT GamePlayLevel::Render()
{
	return S_OK;
}
