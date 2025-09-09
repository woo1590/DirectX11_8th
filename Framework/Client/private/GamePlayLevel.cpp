#include "pch.h"
#include "GamePlayLevel.h"
#include "EngineCore.h"

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
	m_strLevelTag = "GamePlay";

	if (FAILED(InitializeLayerGameObject("Layer_GameObject")))
		return E_FAIL;

	if (FAILED(IntializeLayerCamera("Layer_Camera")))
		return E_FAIL;

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
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"GamePlay Level");

	return S_OK;
}

HRESULT GamePlayLevel::IntializeLayerCamera(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_FreeCam",
								 ENUM_CLASS(LevelID::GamePlay), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT GamePlayLevel::InitializeLayerGameObject(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	/*if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_TestCube",
								 ENUM_CLASS(LevelID::GamePlay), layerTag)))
		return E_FAIL;*/

	if(FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static),"Prototype_Object_Terrain",
								ENUM_CLASS(LevelID::GamePlay),layerTag)))
		return E_FAIL;

	return S_OK;
}
