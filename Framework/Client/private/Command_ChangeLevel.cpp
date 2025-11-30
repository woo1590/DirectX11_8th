#include "pch.h"
#include "Command_ChangeLevel.h"
#include "EngineCore.h"
#include "Level.h"

Command_ChangeLevel::Command_ChangeLevel(LevelID nextLevelID, Level* nextLevel)
	:m_pChangeLevel{nextLevel}, m_eNextLevelID(nextLevelID)
{
}

Command_ChangeLevel* Command_ChangeLevel::Create(LevelID nextLevelID, Level* nextLevel)
{
	Command_ChangeLevel* Instance = new Command_ChangeLevel(nextLevelID, nextLevel);

	return Instance;
}

void Command_ChangeLevel::Execute()
{
	auto engine = EngineCore::GetInstance();

	engine->ChangeLevel(ENUM_CLASS(m_eNextLevelID), m_pChangeLevel);
}

void Command_ChangeLevel::Free()
{
	__super::Free();
}
