#include "pch.h"
#include "BossStoneProjectile.h"
#include "Bounding_Sphere.h"
#include "Random.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"

BossStoneProjectile::BossStoneProjectile()
	:Projectile()
{
}

BossStoneProjectile::BossStoneProjectile(const BossStoneProjectile& prototype)
	:Projectile(prototype)
{
}

BossStoneProjectile* BossStoneProjectile::Create()
{
	BossStoneProjectile* Instance = new BossStoneProjectile();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossStoneProjectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<ModelComponent>();

	return S_OK;
}

HRESULT BossStoneProjectile::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::BossStoneProjectile);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 15.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/	
	_uint rand = engine->GetRandom()->get<_uint>(0, 1);
	auto model = GetComponent<ModelComponent>();
	if(rand)
		model->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_Boss_Stone_Projectile0");
	else
		model->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_Boss_Stone_Projectile1");

	ChangeState(&m_BossStoneProjectileSpawn);

	m_pTarget = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	m_pTarget->AddRef();

	m_fLifeTime = 20.f;
	m_fSpeed = 100.f;

	return S_OK;
}

void BossStoneProjectile::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossStoneProjectile::Update(_float dt)
{
	__super::Update(dt);

	m_pTransform->Turn(_float3{ math::ToRadian(-360.f * dt * 1.5f),math::ToRadian(100.f * dt),0.f });

	if (m_IsActive)
	{
		_float3 targetPosition = m_pTarget->GetComponent<TransformComponent>()->GetPosition();
		_float3 currPosition = m_pTransform->GetPosition();
		_float3 nextPosition{};
		_float3 targetDir{};
		XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition)));
		XMStoreFloat3(&m_CurrDirection, XMVectorLerp(XMLoadFloat3(&m_CurrDirection), XMLoadFloat3(&targetDir), dt * 5.f));
		XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&m_CurrDirection) * m_fSpeed * dt);

		m_pTransform->SetPosition(nextPosition);
	}
}

void BossStoneProjectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void BossStoneProjectile::OnCollisionEnter(ColliderComponent* otherCollider)
{
	SetDead();
}

Object* BossStoneProjectile::Clone(InitDESC* arg)
{
	BossStoneProjectile* Instance = new BossStoneProjectile(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossStoneProjectile::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	Safe_Release(m_pTarget);
	__super::Free();
}

void BossStoneProjectile::BossStoneProjectileSpawn::Enter(Object* object)
{
	auto random = EngineCore::GetInstance()->GetRandom();

	m_fElapsedTime = 0.f;
	m_fDelayTimer = 0.f;
	m_fDelayDuration = m_fDuration + random->get<_float>(1.f, 3.5f);

	m_StartScale = _float3{ 0.1f,0.1f,0.1f };
	m_EndScale = _float3{ 0.4f,0.4f,0.4f };
}

void BossStoneProjectile::BossStoneProjectileSpawn::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
	m_fDelayTimer += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutQuint(t);

		_float3 currScale{};
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_EndScale), t));

		object->GetComponent<TransformComponent>()->SetScale(currScale);
	}
}

void BossStoneProjectile::BossStoneProjectileSpawn::TestForExit(Object* object)
{
	if (m_fDelayTimer >= m_fDelayDuration)
	{
		auto stone = static_cast<BossStoneProjectile*>(object);
		stone->ChangeState(&stone->m_BossStoneProjectileIdle);
		stone->m_IsActive = true;
	}
}
