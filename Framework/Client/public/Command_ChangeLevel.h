#pragma once
#include "ICommand.h"

NS_BEGIN(Engine)
class Level;
NS_END

NS_BEGIN(Client)

class Command_ChangeLevel :
    public ICommand
{
private:
    Command_ChangeLevel(LevelID nextLevelID, Level* nextLevel);
    virtual ~Command_ChangeLevel() = default;

public:
    static Command_ChangeLevel* Create(LevelID nextLevelID, Level* nextLevel);
    void Execute() override;
    void Free()override;

private:
    LevelID m_eNextLevelID{};
    Level* m_pChangeLevel = nullptr;
};

NS_END