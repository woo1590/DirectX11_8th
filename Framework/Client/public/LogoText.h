#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class LogoText :
    public UIObject
{
private:
    LogoText();
    LogoText(const LogoText& prototype);
    virtual ~LogoText() = default;

public:
    static LogoText* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

};

NS_END