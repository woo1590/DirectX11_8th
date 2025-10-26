#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Door;
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
        _int doorID = -1;
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

    void OnCollisionEnter(ColliderComponent* otherCollider)override;
    void OnCollisionStay(ColliderComponent* otherCollider)override {};
    void OnCollisionExit(ColliderComponent* otherCollider)override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    std::vector<_uint> m_AvailableNavCellIndices;
    std::vector<Wave> m_Waves;
    std::list<Object*> m_CurrWaveEnemies;
    Door* m_pConnectedDoor = nullptr;
    _uint m_iCurrWave{};

    class SpawnerIdle : public State
    {
        void Enter(Object* object)override {};
        void Update(Object* object, _float dt)override {};
        void TestForExit(Object* object)override {};
    };
    class SpawnerSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override {};
        void TestForExit(Object* object)override;
    };
    class SpawnerWaveRunning : public State
    {
        void Enter(Object* object)override {};
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class SpawnerEnd : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override {};
        void TestForExit(Object* object)override {};
    };

    SpawnerIdle m_SpawnerIdle;
    SpawnerSpawn m_SpawnerSpawn;
    SpawnerWaveRunning m_SpawnerWaveRunning;
    SpawnerEnd m_SpawnerEnd;
};

NS_END