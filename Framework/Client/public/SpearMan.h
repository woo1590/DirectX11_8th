#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class SpearMan :
    public Enemy
{
private:
    SpearMan();
    SpearMan(const SpearMan& prototype);
    virtual ~SpearMan() = default;

public:
    static SpearMan* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END