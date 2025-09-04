#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL FreecamComponent :
    public Component
{
private:
    FreecamComponent(Object* owner);
    FreecamComponent(const FreecamComponent& prototype);
    virtual ~FreecamComponent() = default;

public:
    static FreecamComponent* Create(Object* owner);
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    Component* Clone()override { return new FreecamComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:

};

NS_END