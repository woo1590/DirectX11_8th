#include "pch.h"
#include "HorseHead_Head.h"
#include "Bounding_Sphere.h"

//component
#include "ColliderComponent.h"

HorseHead_Head::HorseHead_Head()
	:PartObject()
{
}

HorseHead_Head::HorseHead_Head(const HorseHead_Head& prototype)
	:PartObject(prototype)
{
}

HorseHead_Head* HorseHead_Head::Create()
{
	HorseHead_Head* Instance = new HorseHead_Head();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT HorseHead_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "HorseHead_Head";

	return S_OK;
}

HRESULT HorseHead_Head::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 0.f,0.3f,0.f };
	sphereDesc.radius = 0.5f;

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);

	return S_OK;
}

void HorseHead_Head::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void HorseHead_Head::Update(_float dt)
{
	__super::Update(dt);
}

void HorseHead_Head::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* HorseHead_Head::Clone(InitDESC* arg)
{
	HorseHead_Head* Instance = new HorseHead_Head(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void HorseHead_Head::Free()
{
	__super::Free();
}
