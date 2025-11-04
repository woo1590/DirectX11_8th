#include "pch.h"
#include "Soldier_Head.h"
#include "Bounding_Sphere.h"

//component
#include "ColliderComponent.h"

Soldier_Head::Soldier_Head()
	:PartObject()
{
}

Soldier_Head::Soldier_Head(const Soldier_Head& prototype)
	:PartObject(prototype)
{
}

Soldier_Head* Soldier_Head::Create()
{
	Soldier_Head* Instance = new Soldier_Head();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Soldier_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Soldier_Head";

	return S_OK;
}

HRESULT Soldier_Head::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 2.8f,0.f,0.f };
	sphereDesc.radius = 3.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	return S_OK;
}
void Soldier_Head::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Soldier_Head::Update(_float dt)
{
	__super::Update(dt);
}

void Soldier_Head::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Soldier_Head::Clone(InitDESC* arg)
{
	Soldier_Head* Instance = new Soldier_Head(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Soldier_Head::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
