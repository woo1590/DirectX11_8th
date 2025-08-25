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
    virtual ~CameraComponent() = default;

public:
    static CameraComponent* Create(Object* pOnwer, InitDESC* arg);
    HRESULT Initialize(InitDESC* arg)override;
    void Free()override;

    _matrix XM_CALLCONV GetViewMatrix()const;
    _matrix XM_CALLCONV GetProjMatrix()const;

    Component* Clone(InitDESC* arg) { return nullptr; }

private:
    _float m_fAspect{};
    _float m_fFov{};
    _float m_fNearZ{};
    _float m_fFarZ{};

};

NS_END