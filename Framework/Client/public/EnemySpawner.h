#pragma once
#include "Object.h"

NS_BEGIN(Client)

class EnemySpawner :
    public Object
{
public:
    typedef struct tagSpawnEntry
    {
        _string prototypeTag;
        _uint count{};
    }SPAWN_ENTRY;
    using Wave = std::vector<SPAWN_ENTRY>;

    typedef struct tagEnemySpawnerDesc : public Object::OBJECT_DESC
    {
        std::vector<_uint> availableNavCellIndices;
        std::vector<Wave> waves;
    }ENEMY_SPAWNER_DESC;
private:
    EnemySpawner();
    EnemySpawner(const EnemySpawner& prototype);
    virtual ~EnemySpawner() = default;

public:
    static EnemySpawner* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* collider, ColliderComponent* otherCollider)override;
    void OnCollisionStay(ColliderComponent* collider, ColliderComponent* otherCollider)override {};
    void OnCollisionExit(ColliderComponent* collider, ColliderComponent* otherCollider)override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    void Spawn();
    
    _bool m_IsSpawned = false;

    std::vector<_uint> m_AvailableNavCellIndices;
    std::vector<Wave> m_Waves;
};

NS_END