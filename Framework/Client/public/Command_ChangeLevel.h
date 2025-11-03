#pragma once
#include "ICommand.h"

NS_BEGIN(Client)

class Command_ChangeLevel :
    public ICommand
{
private:
    Command_ChangeLevel(LevelID nextLevelID);
    virtual ~Command_ChangeLevel() = default;

public:
    static Command_ChangeLevel* Create(LevelID nextLevelID);
    void Execute() override;
    void Free()override;

private:
    LevelID m_eNextLevelID;
};

NS_END