#include "pch.h"
#include "HorseHead_Sword.h"
#include "Bounding_OBB.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"

HorseHead_Sword::HorseHead_Sword()
	:PartObject()
{
}

HorseHead_Sword::HorseHead_Sword(const HorseHead_Sword& prototype)
	:PartObject(prototype)
{
}

HorseHead_Sword* HorseHead_Sword::Create()
{
	HorseHead_Sword* Instance = new HorseHead_Sword();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT HorseHead_Sword::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "HorseHead_Sword";

	return S_OK;
}

HRESULT HorseHead_Sword::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyAttack);
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 0.f,0.f,-1.5f };
	obbDesc.halfSize = _float3{ 0.2f,0.2f,1.1f };

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);

	/*status*/
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 10.f;
	auto status = GetComponent<StatusComponent>();
	status->Initialize(&statusDesc);

	return S_OK;
}

void HorseHead_Sword::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void HorseHead_Sword::Update(_float dt)
{
	__super::Update(dt);
}

void HorseHead_Sword::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* HorseHead_Sword::Clone(InitDESC* arg)
{
	HorseHead_Sword* Instance = new HorseHead_Sword(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void HorseHead_Sword::Free()
{
	__super::Free();
}
