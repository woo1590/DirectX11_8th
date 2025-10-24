#pragma once
#include "Object.h"

NS_BEGIN(MapEditor)

class PickingSystem;
class EnemySpawner :
    public Object
{
public: 
    typedef struct tagEnemySpawnerDesc : public Object::OBJECT_DESC
    {
        std::vector<_uint> navCellIndices;
    }ENEMY_SPAWNER_DESC;
private:
    EnemySpawner();
    EnemySpawner(const EnemySpawner& prototype);
    virtual ~EnemySpawner() = default;

public:
    static EnemySpawner* Create(PickingSystem* picking);
    HRESULT Initialize_Prototype(PickingSystem* picking);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void AddNavCellIndex(_uint index) { m_AvailableNavCellIndices.push_back(index); }
    ENEMY_SPAWNER_DESC ExportDesc();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    std::vector<_uint> m_AvailableNavCellIndices;

    PickingSystem* m_pPickingSystem = nullptr;
};

NS_END