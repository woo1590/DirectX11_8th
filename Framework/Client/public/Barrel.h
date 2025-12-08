#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Barrel :
    public Object
{
private:
    Barrel();
    Barrel(const Barrel& prototype);
    virtual ~Barrel() = default;

public:
    static Barrel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    void SetDead()override;
    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END