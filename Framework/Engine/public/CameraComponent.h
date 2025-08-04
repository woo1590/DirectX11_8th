#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class TransformComponent;
class ENGINE_DLL CameraComponent :
    public Component
{
private:
    CameraComponent(Object* owner);
    virtual ~CameraComponent() = default;

public:
    static CameraComponent* Create(Object* owner);
    HRESULT Initialize();
    void Free()override;

    DirectX::XMMATRIX GetViewMatrix()const;
    DirectX::XMMATRIX GetProjMatrix()const;

private:
    TransformComponent* target = nullptr;

    _float aspect{};
    _float fov{};
    _float nearZ{};
    _float farZ{};
};

NS_END