#include "pch.h"
#include "Dynamite.h"
#include "Bounding_Sphere.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "StatusComponent.h"

Dynamite::Dynamite()
	:Projectile()
{
}

Dynamite::Dynamite(const Dynamite& prototype)
	:Projectile(prototype)
{
}

Dynamite* Dynamite::Create()
{
	Dynamite* Instance = new Dynamite();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Dynamite::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "Dynamite";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Dynamite::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	DYNAMITE_DESC* desc = static_cast<DYNAMITE_DESC*>(arg);

	/*collider*/	
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::PlayerProjectile);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 5.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	collider->SetActive(false);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Projectile_Dynamite");

	/*status*/
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 100.f;
	auto status = GetComponent<StatusComponent>();
	status->Initialize(&statusDesc);

	m_fLifeTime = 3.f;
	m_Velocity = desc->velocity;
	ChangeState(&m_DynamiteIdle);

	return S_OK;
}

void Dynamite::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Dynamite::Update(_float dt)
{
	__super::Update(dt);

	if (&m_DynamiteIdle == m_CurrState)
	{
		auto engine = EngineCore::GetInstance();

		m_Velocity.y -= 200.f * dt;

		_float3 currPosition = m_pTransform->GetPosition();
		_float3 nextPosition{};
		XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&m_Velocity) * dt);

		RAY worldRay{};
		_float maxDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));
		worldRay.origin = currPosition;
		XMStoreFloat3(&worldRay.direction, XMVector3Normalize(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));

		RAYCAST_DATA mapHitData = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::StaticMapObject));
		RAYCAST_DATA enemyHitData = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::Enemy));
		RAYCAST_DATA enemyShieldHitData = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::EnemyShield));
		RAYCAST_DATA enemyWeakHitData = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::EnemyWeakness));

		if (mapHitData.isHit || enemyHitData.isHit || enemyShieldHitData.isHit || enemyWeakHitData.isHit)
			ChangeState(&m_DynamiteDead);

		m_pTransform->SetPosition(nextPosition);
		m_pTransform->Turn(_float3{ dt * 10.f,0.f,0.f });
	}
}

void Dynamite::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Dynamite::OnCollisionEnter(ColliderComponent* otherCollider)
{
	ChangeState(&m_DynamiteDead);
}

Object* Dynamite::Clone(InitDESC* arg)
{
	Dynamite* Instance = new Dynamite(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Dynamite::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void Dynamite::DynamiteIdle::Enter(Object* object)
{
}

void Dynamite::DynamiteIdle::Update(Object* object, _float dt)
{
}

void Dynamite::DynamiteIdle::TestForExit(Object* object)
{
}

void Dynamite::DynamiteDead::Enter(Object* object)
{
	auto collider = object->GetComponent<ColliderComponent>();
	collider->SetActive(true);

	auto dynamite = static_cast<Dynamite*>(object);
	dynamite->m_eRenderGroup = RenderGroup::None;

	m_fElapsedTime = 0.f;
}

void Dynamite::DynamiteDead::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;	
}

void Dynamite::DynamiteDead::TestForExit(Object* object)
{
	if (m_fElapsedTime > m_fDuration)
		object->SetDead();
}
