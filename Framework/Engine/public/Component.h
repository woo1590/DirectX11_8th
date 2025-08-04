#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Object;
class ENGINE_DLL Component :
    public Base
{
protected:
    Component(Object* owner);
    virtual ~Component() = default;

public:
    virtual void Update(_float dt) {};
    virtual void LateUpdate(_float dt) {};

protected:
    Object* owner = nullptr;

};

NS_END