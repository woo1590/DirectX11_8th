#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class BossStoneProjectile :
    public Projectile
{
private:
    BossStoneProjectile();
    BossStoneProjectile(const BossStoneProjectile& prototype);
    virtual ~BossStoneProjectile() = default;

public:
    static BossStoneProjectile* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    void OnCollisionEnter(ColliderComponent* otherCollider)override;
    void SetDead()override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _uint m_iHitCount{};
    Object* m_pTarget = nullptr;
    Object* m_pStoneSmoke = nullptr;
    Object* m_pStoneTrail = nullptr;

    _float3 m_CurrDirection{};
    _bool m_IsActive = false;

    _float m_fSmokeSpawnElapsedTime{};
    _float m_fSmokeSpawnDuration = 0.1f;

    class BossStoneProjectileSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float3 m_StartScale{};
        _float3 m_EndScale{};

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 1.5f;
        _float m_fDelayTimer = 0.f;
        _float m_fDelayDuration{};
    };
    class BossStoneProjectileIdle : public State
    {
        void Enter(Object* object)override {};
        void Update(Object* object, _float dt)override {};
        void TestForExit(Object* object)override {};
    };

    BossStoneProjectileSpawn m_BossStoneProjectileSpawn;
    BossStoneProjectileIdle m_BossStoneProjectileIdle;
};

NS_END