#include "pch.h"
#include "BossArmProjectile.h"
#include "Bounding_Sphere.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"

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

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss_Arm_Projectile";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT BossArmProjectile::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	BOSS_ARM_PROJECTILE_DESC* desc = static_cast<BOSS_ARM_PROJECTILE_DESC*>(arg);

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::BossArmProjectile);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 30.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	if (desc->armSide)
		model->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_Boss_Arm_Projectile_L");
	else
		model->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_Boss_Arm_Projectile_R");

	m_pTransform->SetScale(_float3(0.9f, 0.9f, 0.9f));
	m_fLifeTime = 10.f;
	m_fSpeed = 300.f;

	return S_OK;
}

void BossArmProjectile::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossArmProjectile::Update(_float dt)
{
	__super::Update(dt);

	_float3 forward = m_pTransform->GetForward();
	_float3 velocity{};
	XMStoreFloat3(&velocity, XMLoadFloat3(&forward) * m_fSpeed);

	m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
}

void BossArmProjectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void BossArmProjectile::OnCollisionEnter(ColliderComponent* otherCollider)
{
	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case Client::ColliderFilter::Player:
	{
		SetDead();
	}break;
	default:
		break;
	}
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
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
