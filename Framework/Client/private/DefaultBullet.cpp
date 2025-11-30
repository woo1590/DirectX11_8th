#include "pch.h"
#include "DefaultBullet.h"
#include "Bounding_Sphere.h"
#include "Random.h"

//object
#include "DefaultBulletTrail.h"
#include "EffectContainer.h"
#include "Decal.h"
#include "DamageFont.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "StatusComponent.h"

DefaultBullet::DefaultBullet()
	:Projectile()
{
}

DefaultBullet::DefaultBullet(const DefaultBullet& prototype)
	:Projectile(prototype)
{
}

DefaultBullet* DefaultBullet::Create()
{
	DefaultBullet* Instance = new DefaultBullet();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DefaultBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "DefaultBullet";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT DefaultBullet::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	DEFAULT_BULLET_DESC* desc = static_cast<DEFAULT_BULLET_DESC*>(arg);

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	if(desc->isEnemy)
		sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyAttack);
	else
		sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::PlayerProjectile);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 0.3f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	EngineCore::GetInstance()->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Projectile_Default_Bullet");

	/*status*/
	auto status = GetComponent<StatusComponent>();
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 20;
	status->Initialize(&statusDesc);

	/*trail*/
	auto random = engine->GetRandom();

	DefaultBulletTrail::DEFAULT_BULLET_TRAIL_DESC bulletDesc{};
	bulletDesc.startPosition = desc->position;
	if (desc->useRandomColor)
	{
		_uint randNum = random->get<_uint>(0, 2);
		if (randNum == 0)
			bulletDesc.mtrlTag = "Mtrl_TrailBulletGreen";
	}
	Object* trail = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DefaultBulletTrail", &bulletDesc);

	m_pTrail = static_cast<DefaultBulletTrail*>(trail);

	m_fSpeed = 1000.f;

	return S_OK;
}

void DefaultBullet::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DefaultBullet::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();

	_float3 forward = m_pTransform->GetForward();
	_vector velocity = XMLoadFloat3(&forward) * m_fSpeed;

	_float3 currPosition = m_pTransform->GetPosition();
	_float3 nextPosition{};
	XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + velocity * dt);

	RAY worldRay{};
	_float maxDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));
	worldRay.origin = currPosition;
	XMStoreFloat3(&worldRay.direction, XMVector3Normalize(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));

	RAYCAST_DATA result = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::StaticMapObject));
	if (result.isHit)
	{
		//currPos + result.distance 더해서 nextPos 만들고 nextPos에 데칼 생성
		EffectContainer::EFFECT_CONTAINER_DESC desc{};
		desc.position = currPosition;
		desc.surfaceDir = result.hitNormal;
		XMStoreFloat3(&desc.position, XMLoadFloat3(&currPosition) + XMLoadFloat3(&result.hitNormal) * 2.f);

		_float rotZ{};
		_float3 viewNormal{};
		_float4x4 viewMatrix = engine->GetCameraContext().viewMatrix;
		XMStoreFloat3(&viewNormal, XMVector3TransformNormal(XMLoadFloat3(&result.hitNormal), XMLoadFloat4x4(&viewMatrix)));
		_float lenSqrt = viewNormal.x * viewNormal.x + viewNormal.y * viewNormal.y;

		rotZ = std::atan2(-viewNormal.y, viewNormal.x);

		Object* effect = nullptr;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitWall", engine->GetCurrLevelID(), "Layer_Effect", &desc, &effect);

		effect->GetComponent<TransformComponent>()->Rotate(_float3(0.f, 0.f, rotZ));

		/*----decal----*/
		_float3 decalPosition{};
		XMStoreFloat3(&decalPosition, XMLoadFloat3(&currPosition) + result.worldDistance*0.9f * XMLoadFloat3(&worldRay.direction));

		Decal::DECAL_DESC decalDesc{};
		decalDesc.position = decalPosition;
		decalDesc.surfaceDir = result.hitNormal;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Decal", engine->GetCurrLevelID(), "Layer_Effect", &decalDesc);

		SetDead();
	}
	else
		m_pTransform->SetPosition(nextPosition);

	m_pTrail->Update(dt);
	m_pTrail->AddNextPosition(nextPosition);
}

void DefaultBullet::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT DefaultBullet::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	if (m_pTrail)
		m_pTrail->ExtractRenderProxies(proxies);

	return S_OK;
}

void DefaultBullet::OnCollisionEnter(ColliderComponent* otherCollider)
{
	auto engine = EngineCore::GetInstance();

	if (otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::Enemy) ||
		otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::EnemyWeakness)||
		otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::BossPillar))
	{
		_float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();
		_float3 position = m_pTransform->GetPosition();
		_float3 forward{};
		XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

		EffectContainer::EFFECT_CONTAINER_DESC desc{};
		desc.position = m_pTransform->GetPosition();
		desc.forward = forward;

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyHit", engine->GetCurrLevelID(), "Layer_Effect", &desc);

		if(otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::Enemy))
			engine->PublishEvent(ENUM_CLASS(EventID::Hit));
		else if(otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::EnemyWeakness))
			engine->PublishEvent(ENUM_CLASS(EventID::WeaknessHit));
	}
	else if (otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::EnemyShield))
	{
		_float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();
		_float3 position = m_pTransform->GetPosition();
		_float3 forward{};
		XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));
		XMStoreFloat3(&position, XMLoadFloat3(&position) + XMLoadFloat3(&forward) * 10.f);
		EffectContainer::EFFECT_CONTAINER_DESC desc{};
		desc.position = position;
		desc.forward = forward;

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_ShieldHit", engine->GetCurrLevelID(), "Layer_Effect", &desc);
	}

	SetDead();
}

Object* DefaultBullet::Clone(InitDESC* arg)
{
	DefaultBullet* Instance = new DefaultBullet(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DefaultBullet::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	Safe_Release(m_pTrail);
	__super::Free();
}
