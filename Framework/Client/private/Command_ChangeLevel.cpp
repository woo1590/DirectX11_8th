#include "pch.h"
#include "Command_ChangeLevel.h"
#include "EngineCore.h"
#include "LoadingLevel.h"

Command_ChangeLevel::Command_ChangeLevel(LevelID nextLevelID)
	:m_eNextLevelID{nextLevelID}
{
}

Command_ChangeLevel* Command_ChangeLevel::Create(LevelID nextLevelID)
{
	Command_ChangeLevel* Instance = new Command_ChangeLevel(nextLevelID);

	return Instance;
}

void Command_ChangeLevel::Execute()
{
	auto engine = EngineCore::GetInstance();
	engine->ChangeLevel(ENUM_CLASS(LevelID::Loading), LoadingLevel::Create(m_eNextLevelID));
	engine->PublishEvent(ENUM_CLASS(EventID::ChangeLevel));
}

void Command_ChangeLevel::Free()
{
	__super::Free();
}
