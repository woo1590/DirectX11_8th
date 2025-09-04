#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Object;
class ENGINE_DLL Component :
    public Base
{
protected:
    Component(Object* pOwner);
    Component(const Component& prototype);
    virtual ~Component() = default;

public:
    virtual HRESULT Initialize_Prototype() { return S_OK; }
    virtual HRESULT Initialize(InitDESC* arg) { return S_OK; };
    virtual void Free()override {};

    virtual void PriorityUpdate(_float dt) {};
    virtual void Update(_float dt) {};
    virtual void LateUpdate(_float dt) {};

    void SetOwner(Object* owner) { m_pOwner = owner; }

    virtual Component* Clone() = 0;

#ifdef USE_IMGUI
    virtual void RenderInspector() = 0;
#endif

protected:
    Object* m_pOwner = nullptr;

};

NS_END