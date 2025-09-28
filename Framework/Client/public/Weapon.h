#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Weapon abstract:
    public PartObject
{
protected:
    Weapon();
    Weapon(const Weapon& prototype);
    virtual ~Weapon() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;

    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

protected:

};

NS_END