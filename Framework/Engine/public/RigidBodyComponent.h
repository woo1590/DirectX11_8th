#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL RigidBodyComponent :
    public Component
{
public:
    typedef struct tagRigidBodyDesc : public InitDESC
    {
        _float inverseMass;
        _float linearDamping;
        _float angularDamping;
        _float mass;
    }RIGID_BODY_DESC;
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
    void AddAngularImpulse(_float3 impulse);
    void AddImpulse(_float3 impulse);

    void SetInertiaTensor(_float halfSize);
    _float3x3 GetInvInertiaTensor()const { return m_InvInertiaTensor; }

    void SetAngularVelocity(_float3 velocity) { m_AngularVelocity = velocity; }
    _float3 GetAngularVelocity()const { return m_AngularVelocity; }

    void SetVelocity(_float3 velocity) { m_Velocity = velocity; }
    _float3 GetVelocity()const { return m_Velocity; }

    void UseIntegrate() { m_UseIntegrate = true; }
    _bool IsUseIntegrate()const { return m_UseIntegrate; }
    void SetGround(_bool isGrounded);
    _bool IsGround()const { return m_IsGround; }

    Component* Clone() { return new RigidBodyComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    void Integrate(_float dt);

    _bool m_IsGround = true;
    _float3 m_Velocity{};
    _float3 m_AngularVelocity{ 0.f,0.f,0.f };

    _bool m_UseIntegrate = false;
    _float3 m_LastFrameAcceleration{};
    _float3 m_Acceleration{ 0.f,-200.f,0.f };
    _float3x3 m_InertiaTensor{};
    _float3x3 m_InvInertiaTensor{};
    _float3 m_ForceAccum{};
    _float3 m_TorqueAccum{};

    _float m_fLinearDamping{};
    _float m_fAngularDamping{};

    _float m_fMass{};
    _float m_fInverseMass{};

    _float m_fGravity = 200.f;
    _float m_fFriction = 10.f;
};

NS_END