#include "pch.h"
#include "SpearMan_Head.h"
#include "Bounding_Sphere.h"
#include "SpearMan.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"


SpearMan_Head::SpearMan_Head()
	:PartObject()
{
}

SpearMan_Head::SpearMan_Head(const SpearMan_Head& prototype)
	:PartObject(prototype)
{
}

SpearMan_Head* SpearMan_Head::Create()
{
	SpearMan_Head* Instance = new SpearMan_Head();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SpearMan_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	return S_OK;
}

HRESULT SpearMan_Head::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyWeakness);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 1.5f,0.f,0.f };
	sphereDesc.radius = 3.5f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);


	return S_OK;
}

void SpearMan_Head::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SpearMan_Head::Update(_float dt)
{
	__super::Update(dt);
}

void SpearMan_Head::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void SpearMan_Head::OnCollisionEnter(ColliderComponent* otherCollider)
{
	auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();
	static_cast<SpearMan*>(m_pParent)->HitHead(otherStatus->GetDesc().attackPower);
}

Object* SpearMan_Head::Clone(InitDESC* arg)
{
	SpearMan_Head* Instance = new SpearMan_Head(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SpearMan_Head::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
