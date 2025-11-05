#include "pch.h"
#include "SpearMan_Spear.h"
#include "Bounding_OBB.h"

//component
#include "ColliderComponent.h"
#include "StatusComponent.h"

SpearMan_Spear::SpearMan_Spear()
	:PartObject()
{
}

SpearMan_Spear::SpearMan_Spear(const SpearMan_Spear& prototype)
	:PartObject(prototype)
{
}

SpearMan_Spear* SpearMan_Spear::Create()
{
	SpearMan_Spear* Instance = new SpearMan_Spear();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;	
}

HRESULT SpearMan_Spear::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	return S_OK;
}

HRESULT SpearMan_Spear::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyAttack);
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 0.f,0.f,-1.5f };
	obbDesc.halfSize = _float3{ 0.2f,0.2f,1.1f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);
	engine->RegisterCollider(collider);

	return S_OK;
}

void SpearMan_Spear::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SpearMan_Spear::Update(_float dt)
{
	__super::Update(dt);
}

void SpearMan_Spear::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* SpearMan_Spear::Clone(InitDESC* arg)
{
	SpearMan_Spear* Instance = new SpearMan_Spear(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SpearMan_Spear::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
