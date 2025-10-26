#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class ColliderComponent;
class CollisionSystem :
    public Base
{
private:
    typedef struct tagCollisionPair
    {
        ColliderComponent* colliderA;
        ColliderComponent* colliderB;

        tagCollisionPair(ColliderComponent* a, ColliderComponent* b)
        {
            if (a > b)
                std::swap(a, b);

            colliderA = a;
            colliderB = b;
        }

        _bool operator<(const tagCollisionPair& other)const noexcept
        {
            return (colliderA < other.colliderA) || (colliderA == other.colliderA && colliderB < other.colliderB);
        }
    }COLLISION_PAIR;
private:
    CollisionSystem();
    virtual ~CollisionSystem() = default;

public:
    static CollisionSystem* Create(_uint numFilters);
    HRESULT Initialize(_uint numFilters);
    void Update();

    RAYCAST_DATA RayCast(RAY worldRay, _uint rayFilter);
    RAYCAST_DATA RayCast(RAY worldRay, _float maxDistance, _uint rayFilter);
    void AddColliderFilterGroup(_uint left, _uint right);
    void RegisterCollider(ColliderComponent* component);
    void UnRegisterCollider(ColliderComponent* component);

    /*for debug*/
    HRESULT ExtractDebugProxies(std::vector<RenderProxy>& proxies);

    void Free()override;

private:
    _bool CheckFilterGroup(_uint left, _uint right);
    void CheckCollision(_uint left, _uint right);
    void ResolveCollision();
    void CollisionEvent();

    std::vector<std::vector<ColliderComponent*>> m_Colliders;
    std::set<std::pair<_uint, _uint>> m_FilterGroup;
    std::vector<_bool> m_CheckSameFilter;

    std::set<COLLISION_PAIR> m_CurrCollisions;
    std::set<COLLISION_PAIR> m_PrevCollisions;
};

NS_END