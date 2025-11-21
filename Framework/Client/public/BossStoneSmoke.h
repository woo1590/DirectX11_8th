#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossStoneSmoke :
    public Object
{
private:
    BossStoneSmoke();
    BossStoneSmoke(const BossStoneSmoke& prototype);
    virtual ~BossStoneSmoke() = default;

public:
    static BossStoneSmoke* Create();
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
    _uint m_iMaxFrame = 16;
    _float m_fSpeed = 40.f;
    _float m_fUVRatio{};
};

NS_END