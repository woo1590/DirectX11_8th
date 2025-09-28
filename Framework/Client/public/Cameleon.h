#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class Cameleon :
    public Weapon
{
private:
    Cameleon();
    Cameleon(const Cameleon& prototype);
    virtual ~Cameleon() = default;

public:
    static Cameleon* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg);
    void Free()override;
};

NS_END