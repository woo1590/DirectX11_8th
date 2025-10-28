#include "pch.h"
#include "Soldier.h"
#include "Bounding_AABB.h"
#include "Random.h"

//object
#include "Socket.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "NavigationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "StatusComponent.h"

Soldier::Soldier()
	:Enemy()
{
}

Soldier::Soldier(const Soldier& prototype)
	:Enemy(prototype)
{
}

Soldier* Soldier::Create()
{
	Soldier* Instance = new Soldier();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Soldier::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<NavigationComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "Soldier";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Soldier::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(Soldier::Parts::Count);

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,4.5f,0.f };
	aabbDesc.halfSize = _float3{ 4.f,4.5f,4.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Enemy_Soldier");

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Enemy_Soldier");

	model->ConnectAnimator();

	/*navigation*/
	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachTransform();
	nav->AttachRigidBody();
	nav->SpawnInCell(90);

	/*status*/
	auto status = GetComponent<StatusComponent>();
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.hp = 500;
	statusDesc.attackPower = 1;
	statusDesc.shield = 100;
	statusDesc.speed = 40.f;
	status->Initialize(&statusDesc);

	ChangeState(&m_SoldierShow);

	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	return S_OK;
}

void Soldier::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Soldier::Update(_float dt)
{
	__super::Update(dt);

	auto nav = GetComponent<NavigationComponent>();
	nav->MoveByVelocity(dt);
}
	
void Soldier::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Soldier::OnCollisionEnter(ColliderComponent* otherCollider)
{
	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case ColliderFilter::PlayerProjectile:
	{
		auto status = GetComponent<StatusComponent>();
		auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

		status->BeAttacked(otherStatus->GetDesc().attackPower);
		if (0 == status->GetDesc().hp)
			ChangeState(&m_SoldierDead);

		if (m_CurrState == &m_SoldierIdle || m_CurrState == &m_SoldierRun)
		{
			if (m_fElapsedTime >= m_fHitDelay)
			{
				ChangeState(&m_SoldierHitBody);
				m_fElapsedTime = 0.f;
			}
		}
	}break;
	default:
		break;
	}
}

Object* Soldier::Clone(InitDESC* arg)
{
	Soldier* Instance = new Soldier(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Soldier::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

HRESULT Soldier::CreatePartObjects()
{
	/*add head socket*/
	{
	}

	return S_OK;
}

void Soldier::SoldierShow::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Show), false, true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void Soldier::SoldierShow::Update(Object* object, _float dt)
{
}

void Soldier::SoldierShow::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto soldier = static_cast<Soldier*>(object);
		soldier->ChangeState(&soldier->m_SoldierIdle);
	}
}

void Soldier::SoldierIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void Soldier::SoldierIdle::Update(Object* object, _float dt)
{
	
}

void Soldier::SoldierIdle::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance < 100.f)
	{
		auto soldier = static_cast<Soldier*>(object);
		soldier->ChangeState(&soldier->m_SoldierRun);
	}
}

void Soldier::SoldierPatrol::Enter(Object* object)
{
}

void Soldier::SoldierPatrol::Update(Object* object, _float dt)
{
}

void Soldier::SoldierPatrol::TestForExit(Object* object)
{
}

void Soldier::SoldierRun::Enter(Object* object)
{
	_uint rand = EngineCore::GetInstance()->GetRandom()->get<_uint>(0, 1);
	auto animator = object->GetComponent<AnimatorComponent>();
	if (0 == rand)
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run1), true);
	else
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run2), true);
}

void Soldier::SoldierRun::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	position.y = 0.f;
	playerPos.y = 0.f;

	_float3 velocity{};
	_float3 targetDir{};
	_float3 currDir = object->GetComponent<TransformComponent>()->GetForward();

	XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));
	XMStoreFloat3(&currDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 10.f));

	XMStoreFloat3(&velocity, XMLoadFloat3(&currDir) * 50.f);

	object->GetComponent<TransformComponent>()->SetForward(currDir);
	object->GetComponent<RigidBodyComponent>()->SetVelocity(velocity);
}

void Soldier::SoldierRun::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	auto soldier = static_cast<Soldier*>(object);
	if (distance >= 100.f)
	{
		soldier->ChangeState(&soldier->m_SoldierIdle);
	}
	else if (distance < 20.f)
	{
		soldier->ChangeState(&soldier->m_SoldierAttack);
	}
}

void Soldier::SoldierAttack::Enter(Object* object)
{
	_uint rand = EngineCore::GetInstance()->GetRandom()->get<_uint>(0, 1);
	auto animator = object->GetComponent<AnimatorComponent>();
	if (0 == rand)
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1), false);
	else
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack2), false);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void Soldier::SoldierAttack::Update(Object* object, _float dt)
{
}

void Soldier::SoldierAttack::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto soldier = static_cast<Soldier*>(object);
		soldier->ChangeState(&soldier->m_SoldierRun);
	}
}

void Soldier::SoldierHitBody::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::HitBody),false, true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3(0.f, 0.f, 0.f));
}

void Soldier::SoldierHitBody::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto soldier = static_cast<Soldier*>(object);
		soldier->ChangeState(&soldier->m_SoldierIdle);
	}
}

void Soldier::SoldierDead::Enter(Object* object)
{
	object->SetDead();
}

