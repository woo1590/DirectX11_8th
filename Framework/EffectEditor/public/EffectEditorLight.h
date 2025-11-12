#pragma once
#include "Object.h"

class EffectEditorLight :
    public Object
{
private:
    EffectEditorLight();
    EffectEditorLight(const EffectEditorLight& prototype);
    virtual ~EffectEditorLight() = default;

public:
    static EffectEditorLight* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
};

