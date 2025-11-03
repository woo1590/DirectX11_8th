#include "EnginePCH.h"
#include "CollisionSystem.h"

//componenet
#include "ColliderComponent.h"

CollisionSystem::CollisionSystem()
{
}

CollisionSystem* CollisionSystem::Create(_uint numFilters)
{
    CollisionSystem* Instance = new CollisionSystem();

    if (FAILED(Instance->Initialize(numFilters)))
        Safe_Release(Instance);

    return Instance;
}

HRESULT CollisionSystem::Initialize(_uint numFilters)
{
    m_Colliders.resize(numFilters);

    return S_OK;
}

void CollisionSystem::Update()
{
    m_CurrCollisions.clear();
    m_CheckSameFilter.assign(m_Colliders.size(), false);

    for (_uint i = 0; i < m_Colliders.size(); ++i)
    {
        for (_uint j = i; j < m_Colliders.size(); ++j)
        {
            if (!CheckFilterGroup(i, j))
                continue;

            if (i == j)
            {
                if (!m_CheckSameFilter[i])
                     m_CheckSameFilter[i] = true;
                else
                    continue;
            }
            
            CheckCollision(i, j);
        }
    }

    ResolveCollision();
    CollisionEvent();
}

RAYCAST_DATA CollisionSystem::RayCast(RAY worldRay, _uint rayFilter)
{
    RAYCAST_DATA result{};

    if (rayFilter >= m_Colliders.size())
        return result;

    for (_uint i = 0; i < m_Colliders[rayFilter].size(); ++i)
    {
        RAYCAST_DATA data = m_Colliders[rayFilter][i]->RayCast(worldRay);

        if (data.isHit && data.worldDistance < result.worldDistance)
            result = data;
    }

    return result;
}

RAYCAST_DATA CollisionSystem::RayCast(RAY worldRay, _float maxDistance, _uint rayFilter)
{
    RAYCAST_DATA result{};
    result.worldDistance = maxDistance;

    for (_uint i = 0; i < m_Colliders[rayFilter].size(); ++i)
    {
        RAYCAST_DATA data = m_Colliders[rayFilter][i]->RayCast(worldRay);

        if (data.isHit && data.worldDistance < result.worldDistance)
            result = data;
    }

    return result;
}

void CollisionSystem::AddColliderFilterGroup(_uint left, _uint right)
{
    if (left > right)
        std::swap(left, right);

    m_FilterGroup.emplace(left, right);
}

void CollisionSystem::RegisterCollider(ColliderComponent* component)
{
    _uint filter = component->GetFilter();

    m_Colliders[filter].push_back(component);
    component->AddRef();
}

void CollisionSystem::UnRegisterCollider(ColliderComponent* component)
{
    _uint filter = component->GetFilter();

    auto iter = std::find(m_Colliders[filter].begin(), m_Colliders[filter].end(), component);

    if (iter != m_Colliders[filter].end())
    {
        _uint index = std::distance(m_Colliders[filter].begin(), iter);
        Safe_Release(m_Colliders[filter][index]);

        _uint lastIndex = m_Colliders[filter].size() - 1;
        std::swap(m_Colliders[filter][index], m_Colliders[filter][lastIndex]);
        m_Colliders[filter].pop_back();
    }

    for (auto iter = m_PrevCollisions.begin(); iter != m_PrevCollisions.end();)
    {
        if (iter->colliderA == component || iter->colliderB == component)
            iter = m_PrevCollisions.erase(iter);
        else ++iter;
    }
}

HRESULT CollisionSystem::ExtractDebugProxies(std::vector<RenderProxy>& proxies)
{
    for (_uint i = 0; i < m_Colliders.size(); ++i)
    {
        for (auto& collider : m_Colliders[i])
        {
            RenderProxy proxy{};
            proxy.collider = collider;

            proxies.push_back(proxy);
        }
    }

    return S_OK;
}

void CollisionSystem::Free()
{
    for (_uint i = 0; i < m_Colliders.size(); ++i)
    {
        for (auto& collider : m_Colliders[i])
            Safe_Release(collider);
        m_Colliders[i].clear();
    }
    m_Colliders.clear();
}

_bool CollisionSystem::CheckFilterGroup(_uint left, _uint right)
{
    if (left > right)
        std::swap(left, right);

    return m_FilterGroup.find({ left, right }) != m_FilterGroup.end();
}

void CollisionSystem::CheckCollision(_uint left, _uint right)
{
    for (_uint i = 0; i < m_Colliders[left].size(); ++i)
    {
        if (!m_Colliders[left][i]->IsActive())
            continue;

        for (_uint j = 0; j < m_Colliders[right].size(); ++j)
        {
            if (left == right && i == j)
                continue;

            if (!m_Colliders[right][j]->IsActive())
                continue;

            if (m_Colliders[left][i]->Intersect(m_Colliders[right][j]))
            {
                m_CurrCollisions.emplace(m_Colliders[left][i], m_Colliders[right][j]);
            }
        }
    }
}

void CollisionSystem::ResolveCollision()
{
    for (auto& pair : m_CurrCollisions)
        pair.colliderA->ResolveCollision(pair.colliderB);
}

void CollisionSystem::CollisionEvent()
{
    for (const auto& pair : m_CurrCollisions)
    {
        if (!m_PrevCollisions.count(pair))
        {
            pair.colliderA->OnCollisionEnter(pair.colliderB);
            pair.colliderB->OnCollisionEnter(pair.colliderA);
        }
    }

    for (const auto& pair : m_CurrCollisions)
    {
        if (m_PrevCollisions.count(pair))
        {
            pair.colliderA->OnCollisionStay(pair.colliderB);
            pair.colliderB->OnCollisionStay(pair.colliderA);
        }
    }

    for (const auto& pair : m_PrevCollisions)
    {
        if (!m_CurrCollisions.count(pair))
        {
            pair.colliderA->OnCollisionExit(pair.colliderB);
            pair.colliderB->OnCollisionExit(pair.colliderA);
        }
    }

    m_PrevCollisions.swap(m_CurrCollisions);
}
