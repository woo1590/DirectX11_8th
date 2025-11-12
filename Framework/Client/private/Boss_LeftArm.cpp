#include "pch.h"
#include "Boss_LeftArm.h"
#include "Bounding_OBB.h"
#include "ContainerObject.h"
#include "Boss.h"

//component
#include "ColliderComponent.h"
#include "StatusComponent.h"

Boss_LeftArm::Boss_LeftArm()
	:PartObject()
{
}

Boss_LeftArm::Boss_LeftArm(const Boss_LeftArm& prototype)
	:PartObject(prototype)
{
}

Boss_LeftArm* Boss_LeftArm::Create()
{
	Boss_LeftArm* Instance = new Boss_LeftArm();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Boss_LeftArm::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss_LeftArm";

	return S_OK;
}

HRESULT Boss_LeftArm::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::BossArm);
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 5.f,0.f,0.f };
	obbDesc.halfSize = _float3{ 9.f,3.f,3.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);

	engine->RegisterCollider(collider);

	return S_OK;
}

void Boss_LeftArm::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boss_LeftArm::Update(_float dt)
{
	__super::Update(dt);
}

void Boss_LeftArm::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Boss_LeftArm::OnCollisionEnter(ColliderComponent* other)
{
	switch (static_cast<ColliderFilter>(other->GetFilter()))
	{
	case ColliderFilter::PlayerProjectile:
	{
		auto otherStatus = other->GetOwner()->GetComponent<StatusComponent>();

		auto boss = static_cast<Boss*>(m_pParent);
		boss->HitBody(otherStatus->GetDesc().attackPower);
	}break;
	default:
		break;
	}
}

Object* Boss_LeftArm::Clone(InitDESC* arg)
{
	Boss_LeftArm* Instance = new Boss_LeftArm(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boss_LeftArm::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
