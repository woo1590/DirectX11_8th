#include "pch.h"
#include "Boom.h"
#include "Bounding_Sphere.h"
#include "ColliderComponent.h"

Boom::Boom()
	:Object()
{
}

Boom::Boom(const Boom& prototype)
	:Object(prototype)
{
}

Boom* Boom::Create()
{
	Boom* Instance = new Boom();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Boom::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	return S_OK;
}

HRESULT Boom::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/	
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.radius = 20.f;
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Boom);

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);

	engine->RegisterCollider(collider);

	return S_OK;
}

void Boom::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boom::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= 0.2f)
		SetDead();
}

void Boom::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Boom::OnCollisionEnter(ColliderComponent* otherCollider)
{

}

Object* Boom::Clone(InitDESC* arg)
{
	Boom* Instance = new Boom(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boom::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
