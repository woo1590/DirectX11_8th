#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class LevelManager final:
    public Base
{
private:
    LevelManager();
    virtual ~LevelManager() = default;

public:
    static LevelManager* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END