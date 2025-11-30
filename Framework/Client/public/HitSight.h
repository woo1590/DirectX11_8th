#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class HitSight :
    public UIObject
{
public:
    typedef struct tagHitSightDesc : public UIObject::UIOBJECT_DESC
    {
        _float3 hitWorldPosition{};
    }HIT_SIGHT_DESC;
private:
    HitSight();
    HitSight(const HitSight& prototype);
    virtual ~HitSight() = default;

public:
    static HitSight* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fAlphaDuration = 1.f;
    _float m_fPosDuration = 0.3f;
    _float m_fElpasedTime{};
    _float m_fStartRadius = 60.f;
    _float m_fEndRadius = 100.f;
    _float3 m_HitWorldPosition{};

    _float3 m_StartPosition{};
    _float3 m_EndPosition{};
};

NS_END