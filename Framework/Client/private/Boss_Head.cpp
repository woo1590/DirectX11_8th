#include "pch.h"
#include "Boss_Head.h"
#include "Bounding_Sphere.h"
#include "Boss.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"

Boss_Head::Boss_Head()
	:PartObject()
{
}

Boss_Head::Boss_Head(const Boss_Head& prototype)
	:PartObject(prototype)
{
}

Boss_Head* Boss_Head::Create()
{
	Boss_Head* Instance = new Boss_Head();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Boss_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss_Head";

	return S_OK;
}

HRESULT Boss_Head::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyWeakness);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 7.5f,0.f,0.f };
	sphereDesc.radius = 5.5f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);

	engine->RegisterCollider(collider);

	return S_OK;
}

void Boss_Head::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boss_Head::Update(_float dt)
{
	__super::Update(dt);
}

void Boss_Head::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Boss_Head::OnCollisionEnter(ColliderComponent* other)
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

Object* Boss_Head::Clone(InitDESC* arg)
{
	Boss_Head* Instance = new Boss_Head(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boss_Head::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
