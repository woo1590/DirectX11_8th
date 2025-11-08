#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class WeaponIcon :
    public UIObject
{
public:
    typedef struct tagWeaponIconDesc : public UIObject::UIOBJECT_DESC
    {
        WeaponID weaponID = WeaponID::Count;
    }WEAPON_ICON_DESC;
private:
    WeaponIcon();
    WeaponIcon(const WeaponIcon& prototype);
    virtual ~WeaponIcon() = default;

public:
    static WeaponIcon* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END