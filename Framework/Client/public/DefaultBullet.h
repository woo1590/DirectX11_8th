#pragma once
#include "Projectile.h"

NS_BEGIN(Client)

class DefaultBulletTrail;
class DefaultBullet :
    public Projectile
{
public:
    typedef struct tagDefaultBulletDesc : public Object::OBJECT_DESC
    {
        _bool useRandomColor = false;
        _bool isEnemy = false;
    }DEFAULT_BULLET_DESC;
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
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    DefaultBulletTrail* m_pTrail = nullptr;
    _float m_fTrailElapsedTime{};
    _float m_fTrailDuration = 0.05f;
};

NS_END