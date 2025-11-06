#include "pch.h"
#include "PrismProjectile.h"
#include "Bounding_Sphere.h"

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
	statusDesc.attackPower = 10;
	auto status = GetComponent<StatusComponent>();
	status->Initialize(&statusDesc);

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
	}

	m_pTransform->SetPosition(nextPosition);
}

void PrismProjectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
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

	__super::Free();
}
