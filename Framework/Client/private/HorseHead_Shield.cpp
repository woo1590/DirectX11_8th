#include "pch.h"
#include "HorseHead_Shield.h"
#include "Bounding_OBB.h"

//component
#include "ColliderComponent.h"

HorseHead_Shield::HorseHead_Shield()
	:PartObject()
{
}

HorseHead_Shield::HorseHead_Shield(const HorseHead_Shield& prototype)
	:PartObject(prototype)
{
}

HorseHead_Shield* HorseHead_Shield::Create()
{
	HorseHead_Shield* Instance = new HorseHead_Shield();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT HorseHead_Shield::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "HorseHead_Shield";

	return S_OK;
}

HRESULT HorseHead_Shield::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.type = ColliderType::OBB;
	obbDesc.halfSize = _float3{ 1.f,0.8f,0.2f };

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);

	return S_OK;
}

void HorseHead_Shield::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void HorseHead_Shield::Update(_float dt)
{
	__super::Update(dt);
}

void HorseHead_Shield::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* HorseHead_Shield::Clone(InitDESC* arg)
{
	HorseHead_Shield* Instance = new HorseHead_Shield(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void HorseHead_Shield::Free()
{
	__super::Free();
}
