#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL LightComponent :
    public Component
{
private:
    LightComponent(Object* owner);
    LightComponent(const LightComponent& prototype);
    virtual ~LightComponent() = default;

public:
    static LightComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Component* Clone() { return new LightComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
};

NS_END