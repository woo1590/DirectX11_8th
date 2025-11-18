#pragma once
#include "Object.h"

NS_BEGIN(Client)

class PrismTrail :
    public Object
{
private:
    PrismTrail();
    PrismTrail(const PrismTrail& prototype);
    virtual ~PrismTrail() = default;

public:
    static PrismTrail* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END