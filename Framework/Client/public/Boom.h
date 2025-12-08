#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Boom :
    public Object
{
private:
    Boom();
    Boom(const Boom& prototype);
    virtual ~Boom() = default;

public:
    static Boom* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
};

NS_END