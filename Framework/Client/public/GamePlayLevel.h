#pragma once
#include "Level.h"

NS_BEGIN(Client)

class GamePlayLevel :
    public Level
{
private:
    GamePlayLevel();
    virtual ~GamePlayLevel() = default;

public:
    static GamePlayLevel* Create();
    HRESULT Initialize();
    void Free()override;   

private:

};

NS_END