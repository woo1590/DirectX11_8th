#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Object;
class ENGINE_DLL Layer final:
    public Base
{
private:
    Layer();
    virtual ~Layer() = default;

public:
    static Layer* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt);
    void LateUpdate(_float dt);

    void AddObject(Object* object);

private:
    std::list<Object*> objects;

};

NS_END