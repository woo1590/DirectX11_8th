#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Renderer :
    public Base
{
private:
    Renderer();
    virtual ~Renderer() = default;

public:
    static Renderer* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END