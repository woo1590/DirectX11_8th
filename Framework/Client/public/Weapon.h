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

    virtual void Reload() = 0;
    virtual void Fire() = 0;

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

protected:

    _uint m_iBip001_R_Hand_Index{};
};

NS_END