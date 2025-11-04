#include "pch.h"
#include "SpearMan.h"
#include "Random.h"
#include "Bounding_AABB.h"

//object
#include "Fracture.h"

//component
#include "RigidBodyComponent.h"
#include "NavigationComponent.h"
#include "AnimatorComponent.h"
#include "ModelComponent.h"
#include "StatusComponent.h"
#include "ColliderComponent.h"

SpearMan::SpearMan()
	:Enemy()
{
}

SpearMan::SpearMan(const SpearMan& prototype)
	:Enemy(prototype)
{
}

SpearMan* SpearMan::Create()
{
	SpearMan* Instance = new SpearMan();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;	
}

HRESULT SpearMan::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<NavigationComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();


	return S_OK;
}

HRESULT SpearMan::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(SpearMan::Parts::Count);

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.useResolve = true;
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,4.5f,0.f };
	aabbDesc.halfSize = _float3{ 4.f,4.5f,4.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Enemy_SpearMan");

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Enemy_SpearMan");

	model->ConnectAnimator();

	/*navigation*/
	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachTransform();
	nav->AttachRigidBody();
	nav->SpawnInCell(2);

	/*status*/
	auto status = GetComponent<StatusComponent>();
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.hp = 100;
	statusDesc.attackPower = 1;
	statusDesc.shield = 100;
	statusDesc.speed = 40.f;
	status->Initialize(&statusDesc);



	ChangeState(&m_SpearManShow);
	m_pTransform->SetScale(_float3{ 1.4f,1.4f,1.4f });

	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	return S_OK;
}

void SpearMan::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SpearMan::Update(_float dt)
{
	__super::Update(dt);

	auto nav = GetComponent<NavigationComponent>();
	nav->MoveByVelocity(dt);
}

void SpearMan::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void SpearMan::OnCollisionEnter(ColliderComponent* otherCollider)
{
	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case ColliderFilter::PlayerAttack:
	{
		auto status = GetComponent<StatusComponent>();
		auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

		status->BeAttacked(otherStatus->GetDesc().attackPower);
		if (0 == status->GetDesc().hp)
			ChangeState(&m_SpearManDead);

		if (m_CurrState == &m_SpearManIdle || m_CurrState == &m_SpearManRun)
		{
			if (m_fElapsedTime >= m_fHitDelay)
			{
				ChangeState(&m_SpearManHitBody);
				m_fElapsedTime = 0.f;
			}
		}
	}break;
	case ColliderFilter::PlayerProjectile:
	{
		auto status = GetComponent<StatusComponent>();
		auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

		status->BeAttacked(otherStatus->GetDesc().attackPower);
		if (0 == status->GetDesc().hp)
			ChangeState(&m_SpearManDead);

		if (m_CurrState == &m_SpearManIdle || m_CurrState == &m_SpearManRun)
		{
			if (m_fElapsedTime >= m_fHitDelay)
			{
				ChangeState(&m_SpearManHitBody);
				m_fElapsedTime = 0.f;
			}
		}
	}break;
	default:
		break;
	}
}

Object* SpearMan::Clone(InitDESC* arg)
{
	SpearMan* Instance = new SpearMan(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SpearMan::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

HRESULT SpearMan::CreatePartObjects()
{
	return S_OK;
}

void SpearMan::SpearManShow::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Show));
}

void SpearMan::SpearManShow::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManShow::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	if (animator->IsFinished())
	{
		auto spearMan = static_cast<SpearMan*>(object);
		spearMan->ChangeState(&spearMan->m_SpearManIdle);
	}
}

void SpearMan::SpearManIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void SpearMan::SpearManIdle::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManIdle::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance < 100.f)
	{
		auto spearMan = static_cast<SpearMan*>(object);
		spearMan->ChangeState(&spearMan->m_SpearManRun);
	}
}

void SpearMan::SpearManPatrol::Enter(Object* object)
{
}

void SpearMan::SpearManPatrol::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManPatrol::TestForExit(Object* object)
{
}

void SpearMan::SpearManRun::Enter(Object* object)
{
	_uint rand = EngineCore::GetInstance()->GetRandom()->get<_uint>(0, 1);
	
	auto animator = object->GetComponent<AnimatorComponent>();
	if (0 == rand)
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run1),true);
	else
		animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run2),true);
}

void SpearMan::SpearManRun::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	position.y = 0.f;
	playerPos.y = 0.f;

	_float3 originVelocity = object->GetComponent<RigidBodyComponent>()->GetVelocity();
	_float3 velocity{};
	_float3 targetDir{};
	_float3 currDir = object->GetComponent<TransformComponent>()->GetForward();

	XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));
	XMStoreFloat3(&currDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 10.f));

	XMStoreFloat3(&velocity, XMLoadFloat3(&currDir) * 50.f);
	velocity.y = originVelocity.y;

	object->GetComponent<TransformComponent>()->SetForward(currDir);
	object->GetComponent<RigidBodyComponent>()->SetVelocity(velocity);

}

void SpearMan::SpearManRun::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	auto spearMan = static_cast<SpearMan*>(object);
	if (distance >= 100.f)
	{
		spearMan->ChangeState(&spearMan->m_SpearManIdle);
	}
	else if (distance < 20.f)
	{
		_uint rand = engine->GetRandom()->get<_uint>(0, 1);

		if (0 == rand)
			spearMan->ChangeState(&spearMan->m_SpearManAttack);
		else
			spearMan->ChangeState(&spearMan->m_SpearManChargeAttack);
	}
}

void SpearMan::SpearManAttack::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack));

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });
}

void SpearMan::SpearManAttack::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManAttack::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto spearMan = static_cast<SpearMan*>(object);
		spearMan->ChangeState(&spearMan->m_SpearManIdle);
	}
}

void SpearMan::SpearManChargeAttack::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::ChargeAttack));

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });

	m_IsStartAttacked = false;
	m_IsEndAttacked = false;
}

void SpearMan::SpearManChargeAttack::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto transform = object->GetComponent<TransformComponent>();
	_float progress = animator->GetProgress();

	if (progress < 0.5f)
	{
		auto engine = EngineCore::GetInstance();
		auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");

		_float3 currForward = transform->GetForward();
		_float3 targetForward{};

		_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
		_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();
		playerPos.y = position.y;
		XMStoreFloat3(&targetForward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));
		XMStoreFloat3(&currForward, XMVectorLerp(XMLoadFloat3(&currForward), XMLoadFloat3(&targetForward), dt * 5.f));

		transform->SetForward(currForward);
	}


	if (!m_IsStartAttacked && progress >= 0.5f)
	{
		auto rigidBody = object->GetComponent<RigidBodyComponent>();

		_float3 forward = transform->GetForward();
		_float3 velocity{};
		XMStoreFloat3(&velocity, XMLoadFloat3(&forward) * m_fAttackSpeed);

		rigidBody->SetVelocity(velocity);

		m_IsStartAttacked = true;
	}

	if (!m_IsEndAttacked && progress >= 0.7f)
	{
		auto rigidBody = object->GetComponent<RigidBodyComponent>();
		rigidBody->SetVelocity(_float3{0.f,0.f,0.f});

		m_IsEndAttacked = true;
	}
}

void SpearMan::SpearManChargeAttack::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto spearMan = static_cast<SpearMan*>(object);
		spearMan->ChangeState(&spearMan->m_SpearManIdle);
	}
}

void SpearMan::SpearManHitBody::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::HitBody), false, true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3(0.f, 0.f, 0.f));
}

void SpearMan::SpearManHitBody::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManHitBody::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto spearMan = static_cast<SpearMan*>(object);
		spearMan->ChangeState(&spearMan->m_SpearManIdle);
	}
}

void SpearMan::SpearManHitHead::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::HitHead), false, true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3(0.f, 0.f, 0.f));
}

void SpearMan::SpearManHitHead::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManHitHead::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto spearMan = static_cast<SpearMan*>(object);
		spearMan->ChangeState(&spearMan->m_SpearManIdle);
	}
}

void SpearMan::SpearManDead::Enter(Object* object)
{
	object->SetDead();

	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();

	_float3 camPosition = engine->GetCameraContext().camPosition;
	_float3 position = transform->GetPosition();
	_float3 hitDir{};
	XMStoreFloat3(&hitDir, XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&camPosition)));

	Fracture::FRACTURE_DESC desc{};
	desc.scale = _float3{ 1.2f,1.2f,1.2f };
	desc.quaternion = transform->GetQuaternion();

	auto random = engine->GetRandom();
	for (_uint i = 0; i < 17; i += 2)
	{
		desc.position.x = position.x + random->get<_float>(-4.f, 4.f);
		desc.position.y = position.y + random->get<_float>(4.f, 7.f);
		desc.position.z = position.z + random->get<_float>(-4.f, 4.f);

		_float3 dir{};
		_float dirFactor = random->get<_float>(0.4f, 0.6f);
		XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&desc.position) - XMLoadFloat3(&position)));
		XMStoreFloat3(&dir, XMVector3Normalize((XMLoadFloat3(&hitDir) * dirFactor + XMLoadFloat3(&dir) * (1.f - dirFactor))));
		XMStoreFloat3(&dir, XMVector3Normalize((XMLoadFloat3(&dir) + XMVectorSet(0.f, 0.2f, 0.f, 0.f))));

		_float power = random->get<_float>(90.f, 150.f);
		_float3 force{};
		_float3 angularForce{};
		XMStoreFloat3(&force, XMLoadFloat3(&dir) * power);
		XMStoreFloat3(&angularForce, XMLoadFloat3(&dir) * power * 0.1f);

		_string modelTag = "SpearMan" + std::to_string(i);
		desc.modelTag = "Model_Fracture_" + modelTag;

		Object* fracture = nullptr;
		desc.spawnNavCell = object->GetComponent<NavigationComponent>()->GetCurrCellIndex();
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Fracture", engine->GetCurrLevelID(), "Layer_Fracture", &desc, &fracture);

		fracture->GetComponent<RigidBodyComponent>()->AddImpulse(force);
		fracture->GetComponent<RigidBodyComponent>()->AddAngularImpulse(angularForce);
	}
}

void SpearMan::SpearManDead::Update(Object* object, _float dt)
{
}

void SpearMan::SpearManDead::TestForExit(Object* object)
{
}
