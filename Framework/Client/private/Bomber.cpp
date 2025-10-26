#include "pch.h"
#include "Bomber.h"
#include "Bounding_AABB.h"

//component	
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "NavigationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

Bomber::Bomber()
	:Enemy()
{
}

Bomber::Bomber(const Bomber& prototype)
	:Enemy(prototype)
{
}

Bomber* Bomber::Create()
{
	Bomber* Instance = new Bomber();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Bomber::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<NavigationComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Bomber";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Bomber::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,5.f,0.f };
	aabbDesc.halfSize = _float3{ 6.f,10.f,6.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Enemy_Bomber");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Enemy_Bomber");

	model->ConnectAnimator();

	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachTransform();
	nav->AttachRigidBody();

	m_pTransform->SetPosition(_float3{ -24.f,5.f,300.f });
	ChangeState(&m_BomberIdle);
	return S_OK;
}

void Bomber::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Bomber::Update(_float dt)
{
	__super::Update(dt);

	auto nav = GetComponent<NavigationComponent>();
	nav->MoveByVelocity(dt);
}

void Bomber::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Bomber::Clone(InitDESC* arg)
{
	Bomber* Instance = new Bomber(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Bomber::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void Bomber::BomberIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void Bomber::BomberIdle::Update(Object* object, _float dt)
{
}

void Bomber::BomberIdle::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();
	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");

	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance < 60.f)
	{
		auto bomber = static_cast<Bomber*>(object);
		bomber->ChangeState(&bomber->m_BomberRun);
	}
}

void Bomber::BomberPatrol::Enter(Object* object)
{
}

void Bomber::BomberPatrol::Update(Object* object, _float dt)
{
}

void Bomber::BomberPatrol::TestForExit(Object* object)
{
}

void Bomber::BomberRun::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run), true);
}

void Bomber::BomberRun::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();
	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	auto transform = object->GetComponent<TransformComponent>();

	_float3 position = transform->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float3 currDir = transform->GetForward();
	_float3 targetDir{};
	XMStoreFloat3(&targetDir,XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));
	XMStoreFloat3(&currDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 5.f));
	
	_float3 velocity{};
	XMStoreFloat3(&velocity, XMLoadFloat3(&currDir) * m_fSpeed);

	object->GetComponent<RigidBodyComponent>()->SetVelocity(velocity);
	transform->SetForward(currDir);
}

void Bomber::BomberRun::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();
	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");

	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance < 20.f)
	{
		auto bomber = static_cast<Bomber*>(object);
		bomber->ChangeState(&bomber->m_BomberAttack);
	}
	else if (distance >= 60.f)
	{
		auto bomber = static_cast<Bomber*>(object);
		bomber->ChangeState(&bomber->m_BomberIdle);
	}
}

void Bomber::BomberAttack::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1), true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void Bomber::BomberAttack::Update(Object* object, _float dt)
{
}

void Bomber::BomberAttack::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();
	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");

	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance >= 20.f)
	{
		auto bomber = static_cast<Bomber*>(object);
		bomber->ChangeState(&bomber->m_BomberRun);
	}
}
