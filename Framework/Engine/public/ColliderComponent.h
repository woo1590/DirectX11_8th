#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class Bounding;
class ENGINE_DLL ColliderComponent :
    public Component
{
protected:
    ColliderComponent(Object* owner);
    ColliderComponent(const ColliderComponent& prototype);
    virtual ~ColliderComponent() = default;

public:
    static ColliderComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override;

    void SetBoneIndex(_int index) { m_iBoneIndex = index; }
    virtual _bool Intersect(ColliderComponent* other);
    virtual RAYCAST_DATA RayCast(RAY worldRay);

    _uint GetFilter()const { return m_iColliderFilter; }
    _bool IsActive()const { return m_IsActive; }
    void SetActive(_bool active) { m_IsActive = active; }

    void ResolveCollision(ColliderComponent* other);

    void OnCollisionEnter(ColliderComponent* otherCollider);
    void OnCollisionStay(ColliderComponent* otherCollider);
    void OnCollisionExit(ColliderComponent* otherCollider);

    /*for debug*/
    virtual void Draw();

    virtual Component* Clone() { return new ColliderComponent(*this); }
    virtual void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

protected:
    _int m_iBoneIndex = -1;

    _bool m_IsActive = true;
    _bool m_UseResolve = true;
    _uint m_iColliderFilter{};
    ColliderType m_ColliderType = ColliderType::Count;
    Bounding* m_pBounding = nullptr;

    /*for debug*/
    _bool m_IsCollision = false;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
    ID3D11InputLayout* m_pInputLayout = nullptr;

    BasicEffect* m_pEffect = nullptr;
    PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
};

NS_END