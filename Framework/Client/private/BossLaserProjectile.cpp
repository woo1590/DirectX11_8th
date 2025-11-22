#include "pch.h"
#include "BossLaserProjectile.h"
#include "MaterialInstance.h"
#include "Bounding_Sphere.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"

BossLaserProjectile::BossLaserProjectile()
	:Projectile()
{
}

BossLaserProjectile::BossLaserProjectile(const BossLaserProjectile& prototype)
	:Projectile(prototype)
{
}

BossLaserProjectile* BossLaserProjectile::Create()
{
	BossLaserProjectile* Instance = new BossLaserProjectile();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossLaserProjectile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<ModelComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT BossLaserProjectile::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/	
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::BossLaserProjectile);
	sphereDesc.radius = 3.f;
	sphereDesc.type = ColliderType::Sphere;
	
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_BossLaserProjectile");
	
	auto mtrlInstance = model->GetMaterialInstance();
	mtrlInstance->SetPass("BossLaserProjectile_Pass");

	m_fSpeed = 4000.f;
	m_fLifeTime = 2.f;
	m_pTransform->SetScale(_float3{ 2.f,2.f,10.f });
	m_pTransform->SetForward(_float3{ 0.f,0.f,1.f });
	
	return S_OK;
}

void BossLaserProjectile::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossLaserProjectile::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();

	_float3 forward = m_pTransform->GetForward();
	_float3 velocity{};
	XMStoreFloat3(&velocity, XMLoadFloat3(&forward) * m_fSpeed);
	if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&velocity))) > 0.f)
	{
		_float3 currPosition = m_pTransform->GetPosition();
		_float3 nextPosition{};
		XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&velocity) * dt);

		RAY worldRay{};
		_float maxDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));
		worldRay.origin = currPosition;
		XMStoreFloat3(&worldRay.direction, XMVector3Normalize(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));
		RAYCAST_DATA result = engine->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::StaticMapObject));

		if (result.isHit)
			SetDead();

		m_pTransform->SetPosition(nextPosition);
	}
}

void BossLaserProjectile::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void BossLaserProjectile::OnCollisionEnter(ColliderComponent* otherCollider)
{
	SetDead();
}

Object* BossLaserProjectile::Clone(InitDESC* arg)
{
	BossLaserProjectile* Instance = new BossLaserProjectile(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossLaserProjectile::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
	Safe_Release(m_pTrail);
}
