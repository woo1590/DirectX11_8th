#pragma once
#include "Object.h"

NS_BEGIN(Client)

class FreeCam :
    public Object
{
private:
    FreeCam();
    FreeCam(const FreeCam& prototype);
    virtual ~FreeCam() = default;

public:
    static FreeCam* Create();
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