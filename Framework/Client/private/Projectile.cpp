#include "pch.h"
#include "Projectile.h"


Projectile::Projectile()
	:Object()
{
}

Projectile::Projectile(const Projectile& prototype)
	:Object(prototype)
{
}

HRESULT Projectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Projectile::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void Projectile::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Projectile::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fLifeTime)
		SetDead();
}

void Projectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Projectile::Free()
{
	__super::Free();
}
