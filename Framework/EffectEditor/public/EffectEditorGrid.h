#pragma once
#include "Object.h"

NS_BEGIN(EffectEditor)

class EffectEditorGrid :
    public Object
{
private:
    EffectEditorGrid();
    EffectEditorGrid(const EffectEditorGrid& prototype);
    virtual ~EffectEditorGrid() = default;

public:
    static EffectEditorGrid* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END