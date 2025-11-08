#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class DashIcon :
    public UIObject
{
public:
    typedef struct tagDashIcon : public UIObject::UIOBJECT_DESC
    {

    }DASH_ICON_DESC;
private:
    DashIcon();
    DashIcon(const DashIcon& prototype);
    virtual ~DashIcon() = default;

public:
    static DashIcon* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    /*Event*/
    void DashDeactive(std::any dashCoolDown);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime = 0.f;
    _float m_fDashCoolDownDuration{};
    _bool m_IsActive = true;
};

NS_END