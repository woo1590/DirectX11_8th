#pragma once
#include "Object.h"
NS_BEGIN(MapEditor)

class PickingSystem;
class PreviewObject :
    public Object
{
private:
    PreviewObject();
    PreviewObject(const PreviewObject& prototype);
    virtual ~PreviewObject() = default;

public:
    static PreviewObject* Create(PREFAB prefab, PickingSystem* picking);
    HRESULT Initialize_Prototype(PREFAB prefab, PickingSystem* picking);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    PREFAB m_Prefab{};

    PickingSystem* m_pPickingSystem = nullptr;
};

NS_END