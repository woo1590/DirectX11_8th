#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL TransformComponent final:
    public Component
{
public:
    typedef struct TransformDesc : public InitDESC
    {
        _float3 position{ 0.f,0.f,0.f };
        _float3 scale{ 1.f,1.f,1.f };
        _float4 quaternion{ 0.f,0.f,0.f,1.f };
    }TRANSFORM_DESC;

private:
    TransformComponent(Object* pOwner);
    TransformComponent(const TransformComponent& prototype);
    virtual ~TransformComponent() = default;

public:
    static TransformComponent* Create(Object* pOwner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void SetPosition(_float3 position) { m_Position = position; MakeDirty(); }
    void SetScale(_float3 scale) { m_Scale = scale; MakeDirty(); }
    void SetRotation(_float3 rotation);
    void SetQuaternion(_float4 quaternion);
    void SetForward(_float3 direction);
    void SetParent(TransformComponent* parent);
    
    /*API*/
    void Translate(_fvector velocity);
    void Turn(_float deltaPitch, _float deltaYaw); //no roll
    void Turn(_float3 deltaRadian);
    void Rotate(_float3 euler);

    /*Getter*/
    _float3 GetPosition()const { return m_Position; }
    _vector GetPositionV()const { return XMLoadFloat3(&m_Position); }

    _float3 GetScale()const { return m_Scale; }
    _vector GetScaleV()const { return XMLoadFloat3(&m_Scale); }  

    _float4 GetQuaternion()const { return m_Quaternion; }
    _vector GetQuaternionV()const { return XMLoadFloat4(&m_Quaternion); }

    _float3 GetForward()const { return m_Forward; }
    _vector GetForwardV()const { return XMLoadFloat3(&m_Forward); }

    _float3 GetUp()const { return m_Up; }
    _vector GetUpV()const { return XMLoadFloat3(&m_Up); }

    _float3 GetRight()const { return m_Right; }
    _vector GetRightV()const { return XMLoadFloat3(&m_Right); }

    _float4x4 GetWorldMatrix();
    _float4x4 GetWorldMatrixInverse();

    _bool IsDirty()const { return m_isDirty; }

    Component* Clone()override;
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    void ResolveDirty();
    void MakeDirty();
    void MakeChildrenDirty();

    _vector RemoveRoll(_fvector quaternion);

    _float3 m_Position{};
    _float3 m_Scale{ 1.f,1.f,1.f };
    _float4 m_Quaternion{ 0.f,0.f,0.f,1.f };
    _float3 m_Rotation{};   //cached euler only ui

    _float3 m_Forward{};
    _float3 m_Up{};
    _float3 m_Right{};

    _float4x4 m_WorldMatrix{};
    _float4x4 m_WorldMatrixInverse{};

    _bool m_isDirty = true;
    TransformComponent* m_pParent = nullptr;
    std::vector<TransformComponent*> m_Childrens;

};

NS_END