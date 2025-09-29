#pragma once
#include "Object.h"
NS_BEGIN(MapEditor)

class PreviewObject :
    public Object
{
private:
    PreviewObject();
    PreviewObject(const PreviewObject& prototype);
    virtual ~PreviewObject() = default;

public:
    static PreviewObject* Create(PREFAB prefab);
    HRESULT Initialize_Prototype(PREFAB prefab);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    PREFAB m_Prefab{};

};

NS_END