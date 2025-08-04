#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL RenderSystem :
    public Base
{
private:
    RenderSystem();
    virtual ~RenderSystem() = default;

public:
    static RenderSystem* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END