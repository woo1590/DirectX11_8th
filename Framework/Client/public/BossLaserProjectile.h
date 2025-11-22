#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class BossLaserProjectile :
    public Projectile
{
private:
    BossLaserProjectile();
    BossLaserProjectile(const BossLaserProjectile& prototype);
    virtual ~BossLaserProjectile() = default;

public:
    static BossLaserProjectile* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    Object* m_pTrail = nullptr;

};

NS_END