#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class LogoFire :
    public UIObject
{
private:
    LogoFire();
    LogoFire(const LogoFire& prototype);
    virtual ~LogoFire() = default;

public:
    static LogoFire* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END