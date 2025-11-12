#pragma once
#include "Level.h"

NS_BEGIN(EffectEditor)

class EffectEditorLevel :
    public Level
{
private:
    EffectEditorLevel();
    virtual ~EffectEditorLevel() = default;

public:
    static EffectEditorLevel* Create();
    HRESULT Intialize();

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    HRESULT Initialize_Resource();
    HRESULT Initialize_Object();
};

NS_END