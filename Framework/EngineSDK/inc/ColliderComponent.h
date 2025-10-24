#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class Bounding;
class ENGINE_DLL ColliderComponent final :
    public Component
{
private:
    ColliderComponent(Object* owner);
    ColliderComponent(const ColliderComponent& prototype);
    virtual ~ColliderComponent() = default;

public:
    static ColliderComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override;

    _bool Intersect(ColliderComponent* other);
    _bool Intersect(RAY worldRay, _float& distance);
    void SetBoneIndex(_int index) { m_iBoneIndex = index; }
    _uint GetColliderTag()const { return m_iColliderTag; }

    void OnCollisionEnter(ColliderComponent* collider, ColliderComponent* otherCollider);
    void OnCollisionStay(ColliderComponent* collider, ColliderComponent* otherCollider);
    void OnCollisionExit(ColliderComponent* collider, ColliderComponent* otherCollider);

    /*for debug*/
    void Draw();

    Component* Clone() { return new ColliderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    _int m_iBoneIndex = -1;

    _uint m_iColliderTag{};
    Bounding* m_pBounding = nullptr;
    _bool m_IsCollision = false;

    /*for debug*/
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
    ID3D11InputLayout* m_pInputLayout = nullptr;

    BasicEffect* m_pEffect = nullptr;
    PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
};

NS_END