#pragma once
#include "Component.h"
#include "PickingSystem.h"

NS_BEGIN(MapEditor)

class PickableComponent abstract:
    public Component
{
protected:
    PickableComponent(Object* owner);
    PickableComponent(const PickableComponent& prototype);
    virtual ~PickableComponent() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;

    virtual PICK_RESULT IntersectRay(RAY ray) = 0;
    virtual Component* Clone()override = 0;
    virtual void Free()override;

private:

};

NS_END