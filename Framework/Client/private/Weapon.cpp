#include "pch.h"
#include "Weapon.h"

Weapon::Weapon()
	:PartObject()
{
}

Weapon::Weapon(const Weapon& prototype)
	:PartObject(prototype)
{
}

HRESULT Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Weapon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void Weapon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Weapon::Update(_float dt)
{
	__super::Update(dt);
}

void Weapon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Weapon::Free()
{
	__super::Free();
}
