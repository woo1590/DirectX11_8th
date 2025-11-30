#include "pch.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "EngineCore.h"
#include "LogoLevel.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "StageBoss.h"
#include "GamePlayLevel.h"
#include "Command_ChangeLevel.h"

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
	m_fElapsedTime += dt;

	if (m_pLoader->IsFinish() && m_fElapsedTime>=m_fDuration)
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
		case Client::LevelID::Stage2:
		{
			nextLevel = Stage2::Create();
			engine->PublishEvent(ENUM_CLASS(EventID::ChangeLevel));
		}
			break;
		case Client::LevelID::Stage3:
		{
			nextLevel = Stage3::Create();
			engine->PublishEvent(ENUM_CLASS(EventID::ChangeLevel));
		}
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

		auto command = Command_ChangeLevel::Create(m_eNextLevelID, nextLevel);
		engine->RegisterCommand(command);
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

		m_fElapsedTime = 0.f;
		m_fDuration = 1.f;
	}break;
	case Client::LevelID::Stage2:
	{
		BackGround::BACKGROUND_DESC backGroundDesc{};
		backGroundDesc.mtrlTag = "Mtrl_Background_Stage";

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround", &backGroundDesc)))
			return E_FAIL;

		m_fElapsedTime = 0.f;
		m_fDuration = 1.f;
	}break;
	case Client::LevelID::Stage3:
	{
		BackGround::BACKGROUND_DESC backGroundDesc{};
		backGroundDesc.mtrlTag = "Mtrl_Background_Stage";

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround", &backGroundDesc)))
			return E_FAIL;

		m_fElapsedTime = 0.f;
		m_fDuration = 1.f;
	}break;
	case Client::LevelID::StageBoss:
	{
		BackGround::BACKGROUND_DESC backGroundDesc{};
		backGroundDesc.mtrlTag = "Mtrl_Background_Boss";

		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround", &backGroundDesc)))
			return E_FAIL;

		m_fElapsedTime = 0.f;
		m_fDuration = 1.f;
	}break;
	default:
		break;
	}

	return S_OK;
}
