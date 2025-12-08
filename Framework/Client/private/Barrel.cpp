#include "pch.h"
#include "Barrel.h"
#include "Bounding_AABB.h"

#include "ModelComponent.h"
#include "ColliderComponent.h"
#include <EffectContainer.h>

Barrel::Barrel()
	:Object()
{
}

Barrel::Barrel(const Barrel& prototype)
	:Object(prototype)
{
}

Barrel* Barrel::Create()
{
	Barrel* Instance = new Barrel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Barrel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();

	return S_OK;
}

HRESULT Barrel::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Barrel");

	/*collider*/
	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Barrel);
	aabbDesc.halfSize = _float3{ 5.f,5.f,5.f };
	aabbDesc.center = _float3{ 0.f,5.f,0.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	//m_pTransform->SetPosition(_float3{ -2.f,0.f, 143.f });

	return S_OK;
}

void Barrel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Barrel::Update(_float dt)
{
	__super::Update(dt);
}

void Barrel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Barrel::OnCollisionEnter(ColliderComponent* otherCollider)
{
	if (otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::PlayerProjectile))
	{
		if(!m_isDead)
			SetDead();
	}
}

void Barrel::SetDead()
{
	__super::SetDead();

	auto engine = EngineCore::GetInstance();

	_float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();

	EffectContainer::EFFECT_CONTAINER_DESC desc{};
	desc.position = GetComponent<TransformComponent>()->GetPosition();
	XMStoreFloat3(&desc.forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&desc.position)));

	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Explode", engine->GetCurrLevelID(), "Layer_Effect", &desc);
	engine->Play3DSound("SFX_DynamiteExplode", GetComponent<TransformComponent>()->GetPosition(), 0.6f);

	Object::OBJECT_DESC boomDesc{};
	boomDesc.position = m_pTransform->GetPosition();
	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BarrelBoom", engine->GetCurrLevelID(), "Layer_Boom", &boomDesc);

	engine->MakeShake(0.3f, 0.2f);
}

Object* Barrel::Clone(InitDESC* arg)
{
	Barrel* Instance = new Barrel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Barrel::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}
