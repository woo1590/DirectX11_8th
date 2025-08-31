#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL UIObject abstract:
    public Object
{
protected:
    UIObject();
    virtual ~UIObject() = default;

public:
    virtual void Free()override {};
};

NS_END