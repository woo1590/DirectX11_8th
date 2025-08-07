#pragma once
#include "Level.h"

NS_BEGIN(Client)

class LogoLevel :
    public Level
{
private:
    LogoLevel();
    virtual ~LogoLevel() = default;

public:
    static LogoLevel* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END