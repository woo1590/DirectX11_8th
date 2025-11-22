#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossLaserProjectileTrail :
    public Object
{
public:
    typedef struct tagBossLaserProjectileTrailDesc : public Object::OBJECT_DESC
    {
        _float duration{};
    }BOSS_LASER_PROJECTILE_TRAIL_DESC;
private:
    BossLaserProjectileTrail();
    BossLaserProjectileTrail(const BossLaserProjectileTrail& prototype);
    virtual ~BossLaserProjectileTrail() = default;

public:
    static BossLaserProjectileTrail* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fDuration{};
    _float m_fElapsedTime{};
};

NS_END