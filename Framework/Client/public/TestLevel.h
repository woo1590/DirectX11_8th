#pragma once
#include "Level.h"

NS_BEGIN(Client)

class TestLevel final:
    public Level
{
private:
    TestLevel();
    virtual ~TestLevel() = default;

public:
    static TestLevel* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END