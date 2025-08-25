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

    void Update(_float dt);
    void LateUpdate(_float dt);

    void AddObject(Object* object);

    void SetUpdatable(_bool update) { m_isUpdatable = update; }
    _bool IsUpdatable()const { return m_isUpdatable; }

    void Free()override;

private:
    _bool m_isUpdatable = true;
    std::list<Object*> m_Objects;

};

NS_END