#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class EffectBackground :
    public UIObject
{
    enum class Effect { Hurt, Heal, Count };
private:
    EffectBackground();
    EffectBackground(const EffectBackground& prototype);
    virtual ~EffectBackground() = default;

public:
    static EffectBackground* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    /*Event*/
    void OnHurt(std::any param);
    void OnHeal(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_IsActive = false;
    _float m_fDuration = 0.3f;
    _float m_fElapsedTime = 0.f;

};

NS_END