#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class LightManager :
    public Base
{
private:
    LightManager();
    virtual ~LightManager() = default;

public:
    static LightManager* Create();
    HRESULT Initialize();

    void Free()override;

private:

};

NS_END