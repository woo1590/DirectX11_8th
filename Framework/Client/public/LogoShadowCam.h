#pragma once
#include "Object.h"

NS_BEGIN(Client)

class LogoShadowCam :
    public Object
{
private:
    LogoShadowCam();
    LogoShadowCam(const LogoShadowCam& prototype);
    virtual ~LogoShadowCam() = default;

public:
    static LogoShadowCam* Create();
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