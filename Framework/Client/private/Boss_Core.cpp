#include "pch.h"
#include "Boss_Core.h"
#include "Bounding_Sphere.h"
#include "Boss.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"

Boss_Core::Boss_Core()
	:PartObject()
{
}

Boss_Core::Boss_Core(const Boss_Core& prototype)
	:PartObject(prototype)
{
}

Boss_Core* Boss_Core::Create()
{
	Boss_Core* Instance = new Boss_Core();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);
	
	return Instance;
}

HRESULT Boss_Core::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss_Core";

	return S_OK;
}

HRESULT Boss_Core::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyWeakness);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 1.5f,0.f,0.f };
	sphereDesc.radius = 1.5f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);

	engine->RegisterCollider(collider);

	return S_OK;
}

void Boss_Core::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boss_Core::Update(_float dt)
{
	__super::Update(dt);
}

void Boss_Core::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Boss_Core::OnCollisionEnter(ColliderComponent* other)
{
	switch (static_cast<ColliderFilter>(other->GetFilter()))
	{
	case ColliderFilter::PlayerProjectile:
	{
		auto otherStatus = other->GetOwner()->GetComponent<StatusComponent>();

		auto boss = static_cast<Boss*>(m_pParent);
		boss->HitWeakness(otherStatus->GetDesc().attackPower);
	}break;
	default:
		break;
	}
}

Object* Boss_Core::Clone(InitDESC* arg)
{
	Boss_Core* Instance = new Boss_Core(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boss_Core::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
