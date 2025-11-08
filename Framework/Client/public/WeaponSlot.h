#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class WeaponSlot :
    public UIObject
{
public:
    typedef struct tagWeaponSlotDesc : public UIObject::UIOBJECT_DESC
    {
        _uint slotNum{};
    }WEAPON_SLOT_DESC;
private:
    WeaponSlot();
    WeaponSlot(const WeaponSlot& prototype);
    virtual ~WeaponSlot() = default;

public:
    static WeaponSlot* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void SetActive(_bool active);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_IsActive = false;

    _float m_fDuration = 0.3f;
    _float m_fElapsedTime{};

    _float3 m_StartScale{};
    _float3 m_TargetScale{};
};

NS_END