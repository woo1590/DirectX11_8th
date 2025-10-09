#pragma once
#include "PickableComponent.h"

NS_BEGIN(MapEditor)

class NavPickable :
    public PickableComponent
{
private:
    NavPickable(Object* owner);
    NavPickable(const NavPickable& prototype);
    virtual ~NavPickable() = default;

public:
    static NavPickable * Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    
    PICK_RESULT IntersectRay(RAY ray) override;

    Component* Clone() { return new NavPickable(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif
    
};

NS_END