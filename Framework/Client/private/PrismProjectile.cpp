#include "pch.h"
#include "PrismProjectile.h"
#include "Bounding_Sphere.h"
#include "EffectContainer.h"
#include "PrismTrail.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "StatusComponent.h"

PrismProjectile::PrismProjectile()
	:Projectile()
{
}

PrismProjectile::PrismProjectile(const PrismProjectile& prototype)
	:Projectile(prototype)
{
}

PrismProjectile* PrismProjectile::Create()
{
	PrismProjectile* Instance = new PrismProjectile();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PrismProjectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	return S_OK;
}

HRESULT PrismProjectile::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::PlayerProjectile);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 5.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/	
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_Prism");

	/*status*/
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 40;
	auto status = GetComponent<StatusComponent>();
	status->Initialize(&statusDesc);

	/*trail*/
	Object* trail = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_TrailPrism", nullptr);
	m_pTrail = static_cast<PrismTrail*>(trail);

	m_fLifeTime = 2.5f;
	m_fSpeed = 800.f;

	return S_OK;
}

void PrismProjectile::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PrismProjectile::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();

	_float3 forward = m_pTransform->GetForward();
	_float3 currPosition = m_pTransform->GetPosition();
	_float3 nextPosition{};
	XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&forward) * m_fSpeed * dt);

	RAY worldRay{};
	_float maxDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));
	worldRay.origin = currPosition;
	XMStoreFloat3(&worldRay.direction, XMVector3Normalize(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));

	RAYCAST_DATA result = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::StaticMapObject));
	if (result.isHit)
	{
		_float t = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&forward) * -1.f, XMLoadFloat3(&result.hitNormal)));
		XMStoreFloat3(&forward, XMLoadFloat3(&forward) + 2.f * t * XMLoadFloat3(&result.hitNormal));
		XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&forward) * m_fSpeed * dt);

		m_pTransform->SetForward(forward);

		_float3 viewNormal{};
		_float4x4 viewMatrix = engine->GetCameraContext().viewMatrix;
		XMStoreFloat3(&viewNormal, XMVector3TransformNormal(XMLoadFloat3(&result.hitNormal), XMLoadFloat4x4(&viewMatrix)));
		_float rotZ = std::atan2(-viewNormal.y, viewNormal.x);

		EffectContainer::EFFECT_CONTAINER_DESC testdesc{};
		testdesc.position = m_pTransform->GetPosition();
		testdesc.surfaceDir = result.hitNormal;

		Object* effect = nullptr;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismHitWall", engine->GetCurrLevelID(), "Layer_Effect", &testdesc, &effect);
		effect->GetComponent<TransformComponent>()->Rotate(_float3(0.f, 0.f, rotZ));
	}

	m_pTransform->SetPosition(nextPosition);
	m_pTrail->GetComponent<TransformComponent>()->SetPosition(nextPosition);
	m_pTrail->Update(dt);
}

void PrismProjectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT PrismProjectile::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);
	
	if (m_pTrail)
		m_pTrail->ExtractRenderProxies(proxies);
	
	return S_OK;
}

void PrismProjectile::OnCollisionEnter(ColliderComponent* otherCollider)
{
	SetDead();
}

Object* PrismProjectile::Clone(InitDESC* arg)
{
	PrismProjectile* Instance = new PrismProjectile(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PrismProjectile::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	Safe_Release(m_pTrail);
	__super::Free();
}

void PrismProjectile::CreateEffect(_float dt)
{
	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		auto engine = EngineCore::GetInstance();

		EffectContainer::EFFECT_CONTAINER_DESC desc{};
		desc.position = m_pTransform->GetPosition();
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismFire", engine->GetCurrLevelID(), "Layer_Effect", &desc);

		m_fElapsedTime = 0.f;
	}
}
