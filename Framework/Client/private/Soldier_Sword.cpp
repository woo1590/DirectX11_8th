#include "pch.h"
#include "Soldier_Sword.h"
#include "Bounding_OBB.h"

//component
#include "ColliderComponent.h"
#include "StatusComponent.h"

Soldier_Sword::Soldier_Sword()
	:PartObject()
{
}

Soldier_Sword::Soldier_Sword(const Soldier_Sword& prototype)
	:PartObject(prototype)
{
}

Soldier_Sword* Soldier_Sword::Create()
{
	Soldier_Sword* Instance = new Soldier_Sword();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Soldier_Sword::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "Soldier_Sword";

	return S_OK;
}

HRESULT Soldier_Sword::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyAttack);
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 0.f,0.f,-5.f };
	obbDesc.halfSize = _float3{ 1.f,1.f,5.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);
	collider->SetActive(false);
	engine->RegisterCollider(collider);

	/*status*/
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 10.f;
	auto status = GetComponent<StatusComponent>();
	status->Initialize(&statusDesc);

	return S_OK;
}
void Soldier_Sword::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Soldier_Sword::Update(_float dt)
{
	__super::Update(dt);
}

void Soldier_Sword::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Soldier_Sword::Clone(InitDESC* arg)
{
	Soldier_Sword* Instance = new Soldier_Sword(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Soldier_Sword::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
