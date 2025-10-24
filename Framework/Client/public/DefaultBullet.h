#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class DefaultBullet :
    public Projectile
{
private:
    DefaultBullet();
    DefaultBullet(const DefaultBullet& prototype);
    virtual ~DefaultBullet() = default;

public:
    static DefaultBullet* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime = 0.f;
    _float m_fDuration = 2.f;
};

NS_END