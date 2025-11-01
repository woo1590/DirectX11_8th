#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class BossArmProjectile :
    public Projectile
{
public:
    typedef struct tagBossArmProjectileDesc : public Object::OBJECT_DESC
    {
        _uint armSide{};
    }BOSS_ARM_PROJECTILE_DESC;
private:
    BossArmProjectile();
    BossArmProjectile(const BossArmProjectile& prototype);
    virtual ~BossArmProjectile() = default;

public:
    static BossArmProjectile* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END