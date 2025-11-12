#include "pch.h"
#include "Boss_RightArm.h"
#include "Bounding_OBB.h"
#include "Boss.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"

Boss_RightArm::Boss_RightArm()
	:PartObject()
{
}

Boss_RightArm::Boss_RightArm(const Boss_RightArm& prototype)
	:PartObject(prototype)
{
}

Boss_RightArm* Boss_RightArm::Create()
{
	Boss_RightArm* Instance = new Boss_RightArm();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Boss_RightArm::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss_RightArm";

	return S_OK;
}

HRESULT Boss_RightArm::Initialize(InitDESC* arg)
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

void Boss_RightArm::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boss_RightArm::Update(_float dt)
{
	__super::Update(dt);
}

void Boss_RightArm::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Boss_RightArm::OnCollisionEnter(ColliderComponent* other)
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

Object* Boss_RightArm::Clone(InitDESC* arg)
{
	Boss_RightArm* Instance = new Boss_RightArm(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boss_RightArm::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
