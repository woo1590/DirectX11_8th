#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL AnimatorComponent final:
    public Component
{
private:
    AnimatorComponent(Object* owner);
    AnimatorComponent(const AnimatorComponent& prototype);
    virtual ~AnimatorComponent() = default;

public:
    static AnimatorComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Component* Clone() { return new AnimatorComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:

};

NS_END