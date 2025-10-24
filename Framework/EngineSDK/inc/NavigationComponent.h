#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class TransformComponent;
class RigidBodyComponent;
class NavigationSystem;
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

    void AttachTransform();
    void AttachRigidBody();
    void AttachSystem(NavigationSystem* system);

    /*API*/
    void SpawnInCell(_uint cellIndex);
    void MoveTo(_float3 deltaPosition);
    void MoveByVelocity(_float dt);

    Component* Clone()override { return new NavigationComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    _bool IsMove(_float3 position);
    _float GetHeight(_float3 position);
    _float3 MakeSlideVector(_float3 position, _float3 nextPosition);

    _uint m_iCurrCellIndex{};
    NavigationSystem* m_pNavigationSystem = nullptr;
    TransformComponent* m_pTransform = nullptr;
    RigidBodyComponent* m_pRigidBody = nullptr;
};

NS_END