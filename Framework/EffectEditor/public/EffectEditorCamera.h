#pragma once
#include "Object.h"

NS_BEGIN(EffectEditor)

class EffectEditorCamera :
    public Object
{
private:
    EffectEditorCamera();
    EffectEditorCamera(const EffectEditorCamera& prototype);
    virtual ~EffectEditorCamera() = default;

public:
    static EffectEditorCamera* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_ActiveKeyboard = true;
    _float2 m_fTargetPitchYaw{};
    _float2 m_fCurrentPitchYaw{};

};

NS_END