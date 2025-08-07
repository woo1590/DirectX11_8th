#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Level abstract:
    public Base
{
protected:
    Level();
    virtual ~Level() = default;

public:
    virtual void Update(_float dt);
    virtual HRESULT Render();
    virtual void Free()override;

protected:

};

NS_END