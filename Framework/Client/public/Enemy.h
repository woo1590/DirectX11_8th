#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class Enemy abstract:
    public ContainerObject
{
protected:
    Enemy();
    Enemy(const Enemy& prototype);
    virtual ~Enemy() = default;

public:
    virtual void Update(_float dt)override;

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;
    virtual void SetDead()override;

protected:
    _float m_fElapsedTime = 0.f;
    _float m_fHitDelay = 2.f;

};

NS_END