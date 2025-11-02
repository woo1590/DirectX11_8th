#include "pch.h"
#include "LoadingLevel.h"
#include "Loader.h"
#include "EngineCore.h"
#include "LogoLevel.h"
#include "TestLevel.h"
#include "Stage1.h"
#include "StageBoss.h"
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

	auto engine = EngineCore::GetInstance();
	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", ENUM_CLASS(LevelID::Loading), "Layer_BackGround");

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
		case Client::LevelID::Stage1:
			nextLevel = Stage1::Create();
			break;
		case Client::LevelID::StageBoss:
			nextLevel = StageBoss::Create();
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
