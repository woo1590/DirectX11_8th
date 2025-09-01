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

    void SetActive(_bool active) { m_isActive = active; }
    _bool IsActive()const { return m_isActive; }

    _float4x4 GetViewMatrix()const;
    _float4x4 GetProjMatrix();

    Component* Clone()override { return new CameraComponent(*this); }
    void Free()override;

private:
    _float m_fAspect{};
    _float m_fFov{};
    _float m_fNearZ{};
    _float m_fFarZ{};
    _float4x4 m_ProjMatrix{};

    _bool m_isActive = true;
    _bool m_isDirty = true;

    TransformComponent* m_pTransform = nullptr;
};

NS_END