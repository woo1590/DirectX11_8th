#pragma once
#include "Object.h"

NS_BEGIN(Client)

class DamageFont :
    public Object
{
private:
    DamageFont();
    DamageFont(const DamageFont& prototype);
    virtual ~DamageFont() = default;

public:
    static DamageFont* Create();
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