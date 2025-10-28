#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Projectile abstract:
    public Object
{
protected:
    Projectile();
    Projectile(const Projectile& prototype);
    virtual ~Projectile() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override = 0;
    void Free()override;

protected:
    _float m_fLifeTime = 1.5f;
    _float m_fElapsedTime = 0.f;
};

NS_END