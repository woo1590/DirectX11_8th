#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Sun :
    public Object
{
private:
    Sun();
    Sun(const Sun& prototype);
    virtual ~Sun() = default;

public:
    static Sun* Create();
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