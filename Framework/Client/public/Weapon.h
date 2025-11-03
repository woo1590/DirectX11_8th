#pragma once
#include "PartObject.h"

NS_BEGIN(Engine)
class Skeleton;
NS_END

NS_BEGIN(Client)

class Weapon abstract:
    public PartObject
{
public:
    typedef struct tagWeaponDesc : public PartObject::PART_OBJECT_DESC
    {
    }WEAPON_DESC;

protected:
    Weapon();
    Weapon(const Weapon& prototype);
    virtual ~Weapon() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    virtual void Idle() = 0;
    virtual void Reload() = 0;
    virtual void Fire() = 0;
    virtual void Skill() = 0;
    WeaponID GetWeaponID()const { return m_eWeaponID; }

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

protected:
    _uint m_iBip001_R_Hand_Index{};
    WeaponID m_eWeaponID = WeaponID::Count;
    _uint m_iNumMaxAmmo{};
    _uint m_iNumCurrAmmo{};
};

NS_END