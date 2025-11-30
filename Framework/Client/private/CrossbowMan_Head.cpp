#include "pch.h"
#include "CrossbowMan_Head.h"
#include "Bounding_Sphere.h"
#include "CrossbowMan.h"

//component
#include "ColliderComponent.h"
#include "StatusComponent.h"

CrossbowMan_Head::CrossbowMan_Head()
	:PartObject()
{
}

CrossbowMan_Head::CrossbowMan_Head(const CrossbowMan_Head& prototype)
	:PartObject(prototype)
{
}

CrossbowMan_Head* CrossbowMan_Head::Create()
{
	CrossbowMan_Head* Instance = new CrossbowMan_Head();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CrossbowMan_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	m_strInstanceTag = "CrossbowMan_Head";

	return S_OK;
}

HRESULT CrossbowMan_Head::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::EnemyWeakness);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.center = _float3{ 0.3f,0.f,0.f };
	sphereDesc.radius = 0.4f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	return S_OK;
}

void CrossbowMan_Head::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void CrossbowMan_Head::Update(_float dt)
{
	__super::Update(dt);
}

void CrossbowMan_Head::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void CrossbowMan_Head::OnCollisionEnter(ColliderComponent* otherCollider)
{
	auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();
	static_cast<CrossbowMan*>(m_pParent)->HitHead(otherStatus->GetDesc().attackPower);
}

Object* CrossbowMan_Head::Clone(InitDESC* arg)
{
	CrossbowMan_Head* Instance = new CrossbowMan_Head(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void CrossbowMan_Head::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
