#pragma once
#include "Object.h"

NS_BEGIN(Client)

class EnemySpawner :
    public Object
{
public:
    typedef struct tagEnemySpawnerDesc : public Object::OBJECT_DESC
    {
        std::vector<_uint> availableNavCellIndices;
        std::vector<_string> enemyPrototypeTags;
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
    _bool m_IsSpawned = false;
    void Spawn();

    std::vector<_uint> m_AvailableNavCellIndices;
    std::vector<_string> m_EnemyPrototypeTags;
};

NS_END