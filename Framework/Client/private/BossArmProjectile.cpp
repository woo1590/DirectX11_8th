#include "pch.h"
#include "BossArmProjectile.h"

BossArmProjectile::BossArmProjectile()
	:Projectile()
{
}

BossArmProjectile::BossArmProjectile(const BossArmProjectile& prototype)
	:Projectile(prototype)
{
}

BossArmProjectile* BossArmProjectile::Create()
{
	BossArmProjectile* Instance = new BossArmProjectile();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossArmProjectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT BossArmProjectile::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void BossArmProjectile::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossArmProjectile::Update(_float dt)
{
	__super::Update(dt);
}

void BossArmProjectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void BossArmProjectile::OnCollisionEnter(ColliderComponent* otherCollider)
{
}

Object* BossArmProjectile::Clone(InitDESC* arg)
{
	BossArmProjectile* Instance = new BossArmProjectile(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossArmProjectile::Free()
{
	__super::Free();
}
