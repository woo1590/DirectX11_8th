#include "pch.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "EngineCore.h"
#include "LogoLevel.h"
#include "Stage1.h"
#include "StageBoss.h"
#include "GamePlayLevel.h"

//object
#include "BackGround.h"

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

	if(FAILED(Initialize_LoadingUI()))
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
		auto engine = EngineCore::GetInstance();

		switch (m_eNextLevelID)
		{
		case Client::LevelID::Logo:
			nextLevel = LogoLevel::Create();
			break;
		case Client::LevelID::Stage1:
			nextLevel = Stage1::Create();
			break;
		case Client::LevelID::StageBoss:
		{
			nextLevel = StageBoss::Create();
			engine->PublishEvent(ENUM_CLASS(EventID::ChangeLevel));
		}
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

HRESULT LoadingLevel::Initialize_LoadingUI()
{
	auto engine = EngineCore::GetInstance();
	switch (m_eNextLevelID)
	{
	case Client::LevelID::Logo:
	{
		BackGround::BACKGROUND_DESC backGroundDesc{};
		backGroundDesc.mtrlTag = "Mtrl_Background_Logo";

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround", &backGroundDesc)))
			return E_FAIL;

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoFire", ENUM_CLASS(LevelID::Loading), "Layer_UI")))
			return E_FAIL;
		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoText", ENUM_CLASS(LevelID::Loading), "Layer_UI")))
			return E_FAIL;

	}break;
	case Client::LevelID::Stage1:
	{
		BackGround::BACKGROUND_DESC backGroundDesc{};
		backGroundDesc.mtrlTag = "Mtrl_Background_Stage";

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround", &backGroundDesc)))
			return E_FAIL;
	}break;
	case Client::LevelID::StageBoss:
	{
		BackGround::BACKGROUND_DESC backGroundDesc{};
		backGroundDesc.mtrlTag = "Mtrl_Background_Boss";

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround", &backGroundDesc)))
			return E_FAIL;
	}break;
	default:
		break;
	}

	return S_OK;
}
