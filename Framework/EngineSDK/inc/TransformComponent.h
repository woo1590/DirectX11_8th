#pragma once
#include "Component.h"
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL TransformComponent final:
    public Component
{
public:
    struct TransformDESC : public Object::ObjectDESC
    {
        _float3 position, scale, rotation;
    };

private:
    TransformComponent(Object* pOwner);
    virtual ~TransformComponent() = default;

public:
    static TransformComponent* Create(Object* pOwner, InitDESC* desc);
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    void SetPosition(_float3 position) { m_Position = position; isDirty = true; }
    void SetScale(_float3 scale) { m_Scale = scale; isDirty = true; }
    void SetRotation(_float3 rotation) { m_Rotation = rotation; isDirty = true; }
    void SetForward(_float3 direction);

    void Translate(_fvector velocity);

    _float3 GetPosition()const { return m_Position; }
    _vector GetPositionV()const { return XMLoadFloat3(&m_Position); }

    _float3 GetScale()const { return m_Scale; }
    _vector GetScaleV()const { return XMLoadFloat3(&m_Scale); }  

    _float3 GetRotation()const { return m_Rotation; }
    _vector GetRotationV()const { return XMLoadFloat3(&m_Rotation); }

    _float3 GetForward()const { return m_Forward; }
    _vector GetForwardV()const { return XMLoadFloat3(&m_Forward); }

    _float3 GetUp()const { return m_Up; }
    _vector GetUpV()const { return XMLoadFloat3(&m_Up); }

    _float3 GetRight()const { return m_Right; }
    _vector GetRightV()const { return XMLoadFloat3(&m_Right); }

    _matrix GetWorldMatrix()const;
    _matrix GetWorldMatrixInverse()const;

    Component* Clone(InitDESC* arg)override { return nullptr; };
    void Free()override;

private:
    _float3 m_Position{};
    _float3 m_Scale{};
    _float3 m_Rotation{};   //radian(pitch,yaw,roll)
    _float4 m_Quaternion{}; //cached quternion

    _float3 m_Forward{};
    _float3 m_Up{};
    _float3 m_Right{};

    _float4x4 m_WorldMatrix{};
    
    _bool isDirty = true;
};

NS_END