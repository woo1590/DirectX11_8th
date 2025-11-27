#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class PrismTrail;
class PrismProjectile :
    public Projectile
{
private:
    PrismProjectile();
    PrismProjectile(const PrismProjectile& prototype);
    virtual ~PrismProjectile() = default;

public:
    static PrismProjectile* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    void CreateEffect(_float dt);

    PrismTrail* m_pTrail = nullptr;
    _float m_fElapsedTime{};
    _float m_fDuration = 0.02f;

    _float m_fSoundElpasedTime{};
};

NS_END