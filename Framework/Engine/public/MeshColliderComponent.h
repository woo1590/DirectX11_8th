#pragma once
#include "ColliderComponent.h"

NS_BEGIN(Engine)

class Model;
class ModelComponent;
class ENGINE_DLL MeshColliderComponent :
    public ColliderComponent
{
public:
    typedef struct tagMeshColliderDesc : public InitDESC
    {
        _uint colliderFilter{};
    }MESH_COLLIDER_DESC;
private:
    MeshColliderComponent(Object* owner);
    MeshColliderComponent(const MeshColliderComponent& prototype);
    virtual ~MeshColliderComponent() = default;

public:
    static MeshColliderComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override {};
    _bool Intersect(ColliderComponent* other)override { return false; }
    RAYCAST_DATA RayCast(RAY worldRay)override;

    /*for debug*/
    void Draw()override {};

    HRESULT SetModel(_uint levelID, const _string& modelTag);
    Component* Clone() { return new MeshColliderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    Model* m_pModel = nullptr;
};

NS_END