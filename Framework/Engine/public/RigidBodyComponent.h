#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL RigidBodyComponent :
    public Component
{
private:
    RigidBodyComponent(Object* owner);
    RigidBodyComponent(const RigidBodyComponent& prototype);
    virtual ~RigidBodyComponent() = default;

public:
    static RigidBodyComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override;

    void AddForce(_float3 force);
    void SetVelocity(_float3 velocity) { m_Velocity = velocity; }
    _float3 GetVelocity()const { return m_Velocity; }

    void SetGround(_bool isGrounded);
    _bool IsGround()const { return m_IsGround; }

    Component* Clone() { return new RigidBodyComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    _bool m_IsGround = true;
    _float3 m_Velocity{};
};

NS_END