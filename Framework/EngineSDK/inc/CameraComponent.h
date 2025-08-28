#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class TransformComponent;
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
    static CameraComponent* Create(Object* pOnwer, InitDESC* arg);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Free()override;

    _matrix GetViewMatrix()const;
    _matrix GetProjMatrix()const;

    Component* Clone()override { return new CameraComponent(*this); }

private:
    _float m_fAspect{};
    _float m_fFov{};
    _float m_fNearZ{};
    _float m_fFarZ{};

};

NS_END