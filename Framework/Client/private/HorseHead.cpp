#include "pch.h"
#include "HorseHead.h"
#include "Bounding_AABB.h"
#include "Layer.h"

//object
#include "Socket.h"
#include "HorseHead_Head.h"
#include "HorseHead_Shield.h"
#include "HorseHead_Sword.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "NavigationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "StatusComponent.h"

HorseHead::HorseHead()
	:Enemy()
{
}

HorseHead::HorseHead(const HorseHead& prototype)
	:Enemy(prototype)
{
}

HorseHead* HorseHead::Create()
{
	HorseHead* Instance = new HorseHead();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT HorseHead::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<NavigationComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "HorseHead";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT HorseHead::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(HorseHead::Parts::Count);

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,10.5f,0.f };
	aabbDesc.halfSize = _float3{ 6.f,10.5f,6.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Enemy_HorseHead");

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Enemy_HorseHead");

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
	status->Initialize(&statusDesc);

	ChangeState(&m_HorseHeadShow);

	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	return S_OK;
}

void HorseHead::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void HorseHead::Update(_float dt)
{
	__super::Update(dt);
	
	auto nav = GetComponent<NavigationComponent>();
	nav->MoveByVelocity(dt);
}

void HorseHead::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void HorseHead::OnCollisionEnter(ColliderComponent* otherCollider)
{
	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case ColliderFilter::PlayerProjectile:
	{
		auto status = GetComponent<StatusComponent>();
		auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

		status->BeAttacked(otherStatus->GetDesc().attackPower);
		if (0 == status->GetDesc().hp)
			ChangeState(&m_HorseHeadDead);

		if (m_CurrState == &m_HorseHeadIdle || m_CurrState == &m_HorseHeadWalk_F)
		{
			if (m_fElapsedTime >= m_fHitDelay)
			{
				ChangeState(&m_HorseHeadHitBody);
				m_fElapsedTime = 0.f;
			}
		}
	}break;
	default:
		break;
	}
}

Object* HorseHead::Clone(InitDESC* arg)
{
	HorseHead* Instance = new HorseHead(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void HorseHead::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

HRESULT HorseHead::CreatePartObjects()
{
	/*add shield socket*/
	{
		Socket::SOCKET_DESC shieldSocket{};
		shieldSocket.parent = this;
		shieldSocket.parentModel = GetComponent<ModelComponent>();
		shieldSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bone024");
		shieldSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::Shield_Socket), &shieldSocket)))
			return E_FAIL;
	}

	/*add shield*/
	{
		HorseHead_Shield::HORSE_HEAD_SHIELD_DESC shieldDesc{};
		shieldDesc.parent = this;
		shieldDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Shield_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead_Shield", ENUM_CLASS(Parts::Shield), &shieldDesc)))
			return E_FAIL;
	}

	/*add sword socket*/
	{
		Socket::SOCKET_DESC shieldSocket{};
		shieldSocket.parent = this;
		shieldSocket.parentModel = GetComponent<ModelComponent>();
		shieldSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 Prop1");
		shieldSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::Sword_Socket), &shieldSocket)))
			return E_FAIL;
	}

	/*add sword*/
	{
		HorseHead_Sword::HORSE_HEAD_SWORD_DESC swordDesc{};
		swordDesc.parent = this;
		swordDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Sword_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead_Sword", ENUM_CLASS(Parts::Sword), &swordDesc)))
			return E_FAIL;
	}

	/*add head socket*/
	{
		Socket::SOCKET_DESC headSocket{};
		headSocket.parent = this;
		headSocket.parentModel = GetComponent<ModelComponent>();
		headSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bone014");
		headSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::Head_Socket), &headSocket)))
			return E_FAIL;
	}

	/*add head*/
	{
		HorseHead_Head::HORSE_HEAD_HEAD_DESC headDesc{};
		headDesc.parent = this;
		headDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Head_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead_Head", ENUM_CLASS(Parts::Head), &headDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void HorseHead::HorseHeadShow::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Show), false, true);

	object->GetComponent<RigidBodyComponent>()->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void HorseHead::HorseHeadShow::Update(Object* object, _float dt)
{
}

void HorseHead::HorseHeadShow::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto horse = static_cast<HorseHead*>(object);
		horse->ChangeState(&horse->m_HorseHeadIdle);
	}
}

void HorseHead::HorseHeadIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

	object->GetComponent<RigidBodyComponent>()->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void HorseHead::HorseHeadIdle::Update(Object* object, _float dt)
{
}

void HorseHead::HorseHeadIdle::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance < 100.f)
	{
		auto horse = static_cast<HorseHead*>(object);
		horse->ChangeState(&horse->m_HorseHeadWalk_F);
	}
}

void HorseHead::HorseHeadPatrol::Enter(Object* object)
{
}

void HorseHead::HorseHeadPatrol::Update(Object* object, _float dt)
{
}

void HorseHead::HorseHeadPatrol::TestForExit(Object* object)
{
}

void HorseHead::HorseHeadWalk_F::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Walk_F), true);
}

void HorseHead::HorseHeadWalk_F::Update(Object* object, _float dt)
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
	XMStoreFloat3(&currDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 5.f));

	XMStoreFloat3(&velocity, XMLoadFloat3(&currDir) * 10.f);

	object->GetComponent<TransformComponent>()->SetForward(currDir);
	object->GetComponent<RigidBodyComponent>()->SetVelocity(velocity);
}

void HorseHead::HorseHeadWalk_F::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	auto horse = static_cast<HorseHead*>(object);
	if (distance >= 100.f)
	{
		horse->ChangeState(&horse->m_HorseHeadIdle);
	}
	else if (distance < 40.f)
	{
		horse->ChangeState(&horse->m_HorseHeadAttack1);
	}
}

void HorseHead::HorseHeadAttack1::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1), false);

	object->GetComponent<RigidBodyComponent>()->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void HorseHead::HorseHeadAttack1::Update(Object* object, _float dt)
{
}

void HorseHead::HorseHeadAttack1::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto horse = static_cast<HorseHead*>(object);
		horse->ChangeState(&horse->m_HorseHeadWalk_F);
	}
}

void HorseHead::HorseHeadHitBody::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::HitHead), false, true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3(0.f, 0.f, 0.f));
}

void HorseHead::HorseHeadHitBody::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto horse = static_cast<HorseHead*>(object);
		horse->ChangeState(&horse->m_HorseHeadIdle);
	}
}

void HorseHead::HorseHeadDead::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Die));
}

void HorseHead::HorseHeadDead::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		object->SetDead();
	}
}
