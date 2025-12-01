#include "pch.h"
#include "Beetle_Head.h"
#include "Bounding_Sphere.h"
#include "Beetle.h"

//component
#include "ColliderComponent.h"
#include "StatusComponent.h"

Beetle_Head::Beetle_Head()
	:PartObject()
{
}

Beetle_Head::Beetle_Head(const Beetle_Head& prototype)
	:PartObject(prototype)
{
}

Beetle_Head* Beetle_Head::Create()
{
	Beetle_Head* Instance = new Beetle_Head();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Beetle_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();
	m_strInstanceTag = "Beetle_Head";

	return S_OK;
}

HRESULT Beetle_Head::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyAttack);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 5.f,0.f,0.f };
	sphereDesc.radius = 4.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	collider->SetActive(false);
	engine->RegisterCollider(collider);

	/*status*/
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 10.f;
	auto status = GetComponent<StatusComponent>();
	status->Initialize(&statusDesc);

	return S_OK;
}

void Beetle_Head::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Beetle_Head::Update(_float dt)
{
	__super::Update(dt);
}

void Beetle_Head::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Beetle_Head::OnCollisionEnter(ColliderComponent* otherCollider)
{
}

Object* Beetle_Head::Clone(InitDESC* arg)
{
	Beetle_Head* Instance = new Beetle_Head(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Beetle_Head::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());
	__super::Free();
}
