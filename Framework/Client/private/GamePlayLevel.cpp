#include "pch.h"
#include "GamePlayLevel.h"
#include "EngineCore.h"
#include "Object.h"

//object
#include "EnemySpawner.h"

//component
#include "NavigationComponent.h"

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
	auto engine = EngineCore::GetInstance();

	return S_OK;
}

void GamePlayLevel::Free()
{
	__super::Free();
}

void GamePlayLevel::Update(_float dt)
{
	auto engine = EngineCore::GetInstance();
	
	if (engine->IsKeyPressed('4'))
		engine->SetMainCamera("ThirdCamera");
	if (engine->IsKeyPressed('5'))
		engine->SetMainCamera("PlayerCamera");

}

HRESULT GamePlayLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"GamePlay Level");

	return S_OK;
}
