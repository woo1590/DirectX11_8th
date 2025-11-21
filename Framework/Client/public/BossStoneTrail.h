#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossStoneTrail :
    public Object
{
private:
    BossStoneTrail();
    BossStoneTrail(const BossStoneTrail& prototype);
    virtual ~BossStoneTrail() = default;

public:
    static BossStoneTrail* Create(); 
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    HRESULT LateInitialize()override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fSpriteElapsedTime{};
    _uint m_iCurrFrame{};
    _uint m_iMaxFrame = 8;
    _float m_fSpeed = 30.f;
    _float m_fUVRatio{};
};

NS_END