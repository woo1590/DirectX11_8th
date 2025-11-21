#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class TransformComponent;
class EngineCore;
class ENGINE_DLL CameraComponent :
    public Component
{
public:
    struct CameraDESC :public InitDESC
    {
        _float aspect, fov, nearZ, farZ;
    };
private:
    CameraComponent(Object* pOnwer);
    CameraComponent(const CameraComponent& prototype);
    virtual ~CameraComponent() = default;

public:
    static CameraComponent* Create(Object* pOnwer);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    void SetActive(_bool active) { m_isActive = active; }
    _bool IsActive()const { return m_isActive; }

    _float4x4 GetViewMatrix()const;
    _float4x4 GetProjMatrix();

    _float GetFarZ()const { return m_fFarZ; }
    _float GetNearZ()const { return m_fNearZ; }

    void MakeShake(_float duration, _float power);

    Component* Clone()override { return new CameraComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    _float m_fAspect{};
    _float m_fFov{};
    _float m_fNearZ{};
    _float m_fFarZ{};
    _float4x4 m_ProjMatrix{};

    _bool m_isActive = true;
    _bool m_isDirty = true;

    _float m_fShakeElapsedTime{};
    _float m_fShakeDuration{};
    _float m_fShakePower{};
    _float3 m_ShakeOffset{};
    _float3 m_Frequency{};
    _float3 m_Amp{};
    _float3 m_Phase{};
    _bool m_IsShake = false;

    TransformComponent* m_pTransform = nullptr;
};

NS_END