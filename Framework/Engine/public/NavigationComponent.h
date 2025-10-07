#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class NavMesh;
class ENGINE_DLL NavigationComponent :
    public Component
{
private:
    NavigationComponent(Object* owner);
    NavigationComponent(const NavigationComponent& prototype);
    virtual ~NavigationComponent() = default;

public:
    static NavigationComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Component* Clone()override { return new NavigationComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    NavMesh* m_pNavMesh = nullptr;
};

NS_END