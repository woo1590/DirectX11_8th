#include "pch.h"
#include "BossLaserProjectileTrail.h"
#include "MaterialInstance.h"
#include "Random.h"

//component
#include "TrailComponent.h"

BossLaserProjectileTrail::BossLaserProjectileTrail()
	:Object()
{
}

BossLaserProjectileTrail::BossLaserProjectileTrail(const BossLaserProjectileTrail& prototype)
	:Object(prototype)
{
}

BossLaserProjectileTrail* BossLaserProjectileTrail::Create()
{
	BossLaserProjectileTrail* Instance = new BossLaserProjectileTrail();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossLaserProjectileTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<TrailComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT BossLaserProjectileTrail::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	BOSS_LASER_PROJECTILE_TRAIL_DESC* desc = static_cast<BOSS_LASER_PROJECTILE_TRAIL_DESC*>(arg);

	/*trail*/
	TrailComponent::TRAIL_DESC trailDesc{};
	trailDesc.mode = TrailMode::Default;
	trailDesc.width = engine->GetRandom()->get<_float>(3.f, 8.f);
	
	auto trail = GetComponent<TrailComponent>();
	trail->Initialize(&trailDesc);

	trail->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_BossLaserProjectileTrail");
	trail->GetMaterialInstance()->SetPass("BossLaserProjectileTrail_Pass");

	m_fDuration = desc->duration;

	return S_OK;
}

void BossLaserProjectileTrail::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossLaserProjectileTrail::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		SetDead();
		return;
	}

	auto mtrlInstance = GetComponent<TrailComponent>()->GetMaterialInstance();
	_float t = m_fElapsedTime / m_fDuration;

	mtrlInstance->SetFloat("g_CustomAlpha", 1.f - t);
}

void BossLaserProjectileTrail::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BossLaserProjectileTrail::Clone(InitDESC* arg)
{
	BossLaserProjectileTrail* Instance = new BossLaserProjectileTrail(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossLaserProjectileTrail::Free()
{
	__super::Free();
}
