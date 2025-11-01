#include "pch.h"
#include "Boss.h"
#include "Bounding_AABB.h"
#include "Random.h"

//object
#include "Socket.h"
#include "Boss_Core.h"
#include "Boss_RightArm.h"
#include "Boss_LeftArm.h"
#include "Boss_Head.h"
#include "BossPillar.h"
#include "BossStoneProjectile.h"
#include "BossArmProjectile.h"

//component	
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "ColliderComponent.h"

Boss::Boss()
	:Enemy()
{
}

Boss::Boss(const Boss& prototype)
	:Enemy(prototype)
{
}

Boss::~Boss()
{
}

Boss* Boss::Create()
{
	Boss* Instance = new Boss();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Boss::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Boss::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(Parts::Count);
	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,120.f,20.f };
	aabbDesc.halfSize = _float3{ 50.f,50.f,25.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Enemy_Boss");

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Enemy_Boss");

	model->ConnectAnimator();

	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	m_pTransform->SetPosition(_float3(-38.f, -72.f, 890.f));
	m_pTransform->Rotate(_float3(0.f, math::ToRadian(180.f), 0.f));

	m_PillarAreaMin = _float3{ -190.f,-5.f,410.f };
	m_PillarAreaMax = _float3{ 110.f,-5.f,683.f };
	m_StoneAreaMin = _float3{ -170.f,120.f,800.f };
	m_StoneAreaMax = _float3{ 100.f,160.f,830.f };

	ChangeState(&m_BossIdle);

	return S_OK;
}

void Boss::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boss::Update(_float dt)
{
	__super::Update(dt);

	if (EngineCore::GetInstance()->IsKeyPressed('K'))
		ChangeState(&m_BossIdle);
}

void Boss::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Boss::Dead()
{
	if(m_CurrState != &m_BossDie)
		ChangeState(&m_BossDie);
}

Object* Boss::Clone(InitDESC* arg)
{
	Boss* Instance = new Boss(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boss::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

HRESULT Boss::CreatePartObjects()
{
	/*add core socket*/
	{
		Socket::SOCKET_DESC coreSocket{};
		coreSocket.parent = this;
		coreSocket.parentModel = GetComponent<ModelComponent>();
		coreSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("3909_weakness");
		coreSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::Core_Socket), &coreSocket)))
			return E_FAIL;
	}

	/*add core*/
	{
		Boss_Core::BOSS_CORE_DESC bossCoreDesc{};
		bossCoreDesc.parent = this;
		bossCoreDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Core_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss_Core", ENUM_CLASS(Parts::Core), &bossCoreDesc)))
			return E_FAIL;
	}

	/*add right arm socket*/
	{
		Socket::SOCKET_DESC rightArmSocket{};
		rightArmSocket.parent = this;
		rightArmSocket.parentModel = GetComponent<ModelComponent>();
		rightArmSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 R Forearm");
		rightArmSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::RightArm_Socket), &rightArmSocket)))
			return E_FAIL;
	}

	/*add right arm*/
	{
		Boss_RightArm::BOSS_RIGHT_ARM_DESC rightArmDesc{};
		rightArmDesc.parent = this;
		rightArmDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::RightArm_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss_RightArm", ENUM_CLASS(Parts::RightArm), &rightArmDesc)))
			return E_FAIL;
	}

	/*add left arm socket*/
	{
		Socket::SOCKET_DESC leftArmSocket{};
		leftArmSocket.parent = this;
		leftArmSocket.parentModel = GetComponent<ModelComponent>();
		leftArmSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 L Forearm");
		leftArmSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::LeftArm_Socket), &leftArmSocket)))
			return E_FAIL;
	}
	/*add left arm*/
	{
		Boss_LeftArm::BOSS_LEFT_ARM_DESC leftArmDesc{};
		leftArmDesc.parent = this;
		leftArmDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::LeftArm_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss_LeftArm", ENUM_CLASS(Parts::LeftArm), &leftArmDesc)))
			return E_FAIL;
	}

	/*add head socket*/
	{
		Socket::SOCKET_DESC headSocket{};
		headSocket.parent = this;
		headSocket.parentModel = GetComponent<ModelComponent>();
		headSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 Neck");
		headSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::Head_Socket), &headSocket)))
			return E_FAIL;
	}
	/*add head*/
	{
		Boss_Head::BOSS_HEAD_DESC headDesc{};
		headDesc.parent = this;
		headDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Head_Socket)]->GetComponent<TransformComponent>();
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss_Head", ENUM_CLASS(Parts::Head), &headDesc)))
			return E_FAIL;
	}
	/*add right arm start socket*/
	{
		Socket::SOCKET_DESC rightArmStartSocket{};
		rightArmStartSocket.parent = this;
		rightArmStartSocket.parentModel = GetComponent<ModelComponent>();
		rightArmStartSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 R Hand");
		rightArmStartSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::RightArmStart_Socket), &rightArmStartSocket)))
			return E_FAIL;
	}
	/*add right arm end socket*/
	{
		Socket::SOCKET_DESC rightArmEndSocket{};
		rightArmEndSocket.parent = this;
		rightArmEndSocket.parentModel = GetComponent<ModelComponent>();
		rightArmEndSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 R Finger2");
		rightArmEndSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::RightArmEnd_Socket), &rightArmEndSocket)))
			return E_FAIL;
	}
	/*add left arm start socket*/
	{
		Socket::SOCKET_DESC leftArmStartSocket{};
		leftArmStartSocket.parent = this;
		leftArmStartSocket.parentModel = GetComponent<ModelComponent>();
		leftArmStartSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 L Hand");
		leftArmStartSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::LeftArmStart_Socket), &leftArmStartSocket)))
			return E_FAIL;
	}
	/*add left arm end socket*/
	{
		Socket::SOCKET_DESC leftArmEndSocket{};
		leftArmEndSocket.parent = this;
		leftArmEndSocket.parentModel = GetComponent<ModelComponent>();
		leftArmEndSocket.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 L Finger2");
		leftArmEndSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::LeftArmEnd_Socket), &leftArmEndSocket)))
			return E_FAIL;
	}

	return S_OK;
}

void Boss::BossIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

	m_fElapsedTime = 0.f;
}

void Boss::BossIdle::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;	
}

void Boss::BossIdle::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		_uint rand = EngineCore::GetInstance()->GetRandom()->get<_uint>(0, 11);
		auto boss = static_cast<Boss*>(object);

		if (rand < 4)
			boss->ChangeState(&boss->m_BossAttack1Start);
		else if (rand < 6)
			boss->ChangeState(&boss->m_BossFire1Start);
		else if (rand < 9)
			boss->ChangeState(&boss->m_BossFire2Start);
		else
			boss->ChangeState(&boss->m_BossFire3Start);

	}
}

void Boss::BossAttack1Start::Enter(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1Start));

	auto& pillars = engine->GetObjects(ENUM_CLASS(LevelID::GamePlay), "Layer_BossPillar");
	for (const auto& pillar : pillars)
		static_cast<BossPillar*>(pillar)->Explode();
}

void Boss::BossAttack1Start::Update(Object* object, _float dt)
{
}

void Boss::BossAttack1Start::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossAttack1InProgress);
	}
}

void Boss::BossAttack1InProgress::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1InProgress));
	animator->SetPlaySpeedScale(0.2f);
}

void Boss::BossAttack1InProgress::Update(Object* object, _float dt)
{
}

void Boss::BossAttack1InProgress::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossAttack1End);
	}
}

void Boss::BossAttack1End::Enter(Object* object)
{
	auto random = EngineCore::GetInstance()->GetRandom();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1End));

	if (m_IndexX.empty() && m_IndexZ.empty())
	{
		for (_uint i = 0; i < m_iNumPillars; ++i)
		{
			m_IndexX.push_back(i);
			m_IndexZ.push_back(i);
		}
	}
	random->Shuffle(m_IndexX);
	random->Shuffle(m_IndexZ);

	m_IsPillarSpawned = false;
}

void Boss::BossAttack1End::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (!m_IsPillarSpawned && progress > 0.3f)
	{
		auto boss = static_cast<Boss*>(object);
		_float3 spawnGap{};
		spawnGap.x = (boss->m_PillarAreaMax.x - boss->m_PillarAreaMin.x) / m_iNumPillars;
		spawnGap.y = 0.f;
		spawnGap.z = (boss->m_PillarAreaMax.z - boss->m_PillarAreaMin.z) / m_iNumPillars;

		auto engine = EngineCore::GetInstance();
		auto random = engine->GetRandom();
		for (_uint i = 0; i < m_iNumPillars; ++i)
		{

			Object::OBJECT_DESC pillarDesc{};
			pillarDesc.position.x = boss->m_PillarAreaMin.x + (spawnGap.x * m_IndexX[i]) + random->get<_float>(-5.f, 5.f);
			pillarDesc.position.y = -50.f;
			pillarDesc.position.z = boss->m_PillarAreaMin.z + (spawnGap.z * m_IndexZ[i]) + random->get<_float>(-5.f, 5.f);

			engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_BossPillar", ENUM_CLASS(LevelID::GamePlay), "Layer_BossPillar",&pillarDesc);
		}

		m_IsPillarSpawned = true;
	}
}

void Boss::BossAttack1End::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossIdle);
	}
}

void Boss::BossFire1Start::Enter(Object* object)
{
	auto random = EngineCore::GetInstance()->GetRandom();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1Start));

	if (m_IndexX.empty() && m_IndexZ.empty())
	{
		for (_uint i = 0; i < m_iNumStones; ++i)
		{
			m_IndexX.push_back(i);
			m_IndexY.push_back(i);
			m_IndexZ.push_back(i);
		}
	}
	random->Shuffle(m_IndexX);
	random->Shuffle(m_IndexY);
	random->Shuffle(m_IndexZ);

	m_IsProjectileSpawned = false;
}

void Boss::BossFire1Start::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (!m_IsProjectileSpawned && progress > 0.6f)
	{
		auto random = EngineCore::GetInstance()->GetRandom();
		auto boss = static_cast<Boss*>(object);
		_float3 spawnGap{};
		spawnGap.x = (boss->m_StoneAreaMax.x - boss->m_StoneAreaMin.x) / m_iNumStones;
		spawnGap.y = (boss->m_StoneAreaMax.y - boss->m_StoneAreaMin.y) / m_iNumStones;
		spawnGap.z = (boss->m_StoneAreaMax.z - boss->m_StoneAreaMin.z) / m_iNumStones;

		for (_uint i = 0; i < m_iNumStones; ++i)
		{
			Object::OBJECT_DESC stoneDesc{};
			stoneDesc.position.x = boss->m_StoneAreaMin.x + (spawnGap.x * m_IndexX[i]) + random->get<_float>(-10.f, 10.f);
			stoneDesc.position.y = boss->m_StoneAreaMin.y + (spawnGap.y * m_IndexY[i]) + random->get<_float>(-2.f, 2.f);
			stoneDesc.position.z = boss->m_StoneAreaMin.z + (spawnGap.z * m_IndexZ[i]) + random->get<_float>(-10.f, 10.f);
			auto engine = EngineCore::GetInstance();
			engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_BossStoneProjectile", ENUM_CLASS(LevelID::GamePlay), "Layer_BossStoneProjectile", &stoneDesc);
		}
	
		m_IsProjectileSpawned = true;
	}
}

void Boss::BossFire1Start::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire1InProgress);
	}
}

void Boss::BossFire1InProgress::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1InProgress));
}

void Boss::BossFire1InProgress::Update(Object* object, _float dt)
{
}

void Boss::BossFire1InProgress::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire1End);
	}
}

void Boss::BossFire1End::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1End));
}

void Boss::BossFire1End::Update(Object* object, _float dt)
{
}

void Boss::BossFire1End::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossIdle);
	}
}

void Boss::BossFire2Start::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2Start));
	animator->SetPlaySpeedScale(0.6f);
}

void Boss::BossFire2Start::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();

	_float3 currForward = transform->GetForward();
	_float3 targetForward{};
	_float3 position = transform->GetPosition();
	_float3 playerPosition = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();
	XMStoreFloat3(&targetForward, XMLoadFloat3(&playerPosition) - XMLoadFloat3(&position));
	targetForward.y = 0.f;
	XMStoreFloat3(&targetForward, XMVector3Normalize(XMLoadFloat3(&targetForward)));
	XMStoreFloat3(&targetForward, XMVectorLerp(XMLoadFloat3(&currForward), XMLoadFloat3(&targetForward), dt * 5.f));

	transform->SetForward(targetForward);
}

void Boss::BossFire2Start::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire2Ready);
	}
}

void Boss::BossFire2Ready::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2Ready));
}

void Boss::BossFire2Ready::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();

	_float3 currForward = transform->GetForward();
	_float3 targetForward{};
	_float3 position = transform->GetPosition();
	_float3 playerPosition = engine->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();
	XMStoreFloat3(&targetForward, XMLoadFloat3(&playerPosition) - XMLoadFloat3(&position));
	targetForward.y = 0.f;
	XMStoreFloat3(&targetForward, XMVector3Normalize(XMLoadFloat3(&targetForward)));
	XMStoreFloat3(&targetForward, XMVectorLerp(XMLoadFloat3(&currForward), XMLoadFloat3(&targetForward), dt * 5.f));

	transform->SetForward(targetForward);
}

void Boss::BossFire2Ready::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire2LeftArm);
	}
}

void Boss::BossFire2LeftArm::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2LeftArm));

	auto engine = EngineCore::GetInstance();
	auto boss = static_cast<Boss*>(object);

	_float3 leftArmStartPosition = boss->m_PartObjects[ENUM_CLASS(Parts::LeftArmStart_Socket)]->GetComponent<TransformComponent>()->GetWorldPosition();
	_float3 leftArmEndPosition = boss->m_PartObjects[ENUM_CLASS(Parts::LeftArmEnd_Socket)]->GetComponent<TransformComponent>()->GetWorldPosition();
	_float4 rotation = boss->GetComponent<TransformComponent>()->GetQuaternion();
	XMStoreFloat4(&rotation, XMQuaternionNormalize(XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(math::ToRadian(-8.f), 0.f, math::ToRadian(5.f)))));

	Object* leftArm = nullptr;
	BossArmProjectile::BOSS_ARM_PROJECTILE_DESC leftArmDesc{};
	leftArmDesc.armSide = 1;
	leftArmDesc.position = leftArmStartPosition;
	leftArmDesc.position.x -= 10.f;
	leftArmDesc.position.y -= 10.f;
	leftArmDesc.position.z -= 10.f;
	leftArmDesc.quaternion = rotation;
	engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_BossArmProjectile", ENUM_CLASS(LevelID::GamePlay), "Layer_BossArmProjectile", &leftArmDesc, &leftArm);

}


void Boss::BossFire2LeftArm::Update(Object* object, _float dt)
{
}

void Boss::BossFire2LeftArm::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire2RightArm);
	}
}

void Boss::BossFire2RightArm::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2RightArm));

	auto engine = EngineCore::GetInstance();
	auto boss = static_cast<Boss*>(object);

	_float3 rightArmStartPosition = boss->m_PartObjects[ENUM_CLASS(Parts::RightArmStart_Socket)]->GetComponent<TransformComponent>()->GetWorldPosition();
	_float3 rightArmEndPosition = boss->m_PartObjects[ENUM_CLASS(Parts::RightArmEnd_Socket)]->GetComponent<TransformComponent>()->GetWorldPosition();
	_float4 rotation = boss->GetComponent<TransformComponent>()->GetQuaternion();
	XMStoreFloat4(&rotation, XMQuaternionNormalize(XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(math::ToRadian(-8.f), 0.f, math::ToRadian(-5.f)))));

	Object* rightArm = nullptr;
	BossArmProjectile::BOSS_ARM_PROJECTILE_DESC rightArmDesc{};
	rightArmDesc.armSide = 0;
	rightArmDesc.position = rightArmStartPosition;
	rightArmDesc.position.x += 10.f;
	rightArmDesc.position.y -= 10.f;
	rightArmDesc.position.z -= 10.f;
	rightArmDesc.quaternion = rotation;
	engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_BossArmProjectile", ENUM_CLASS(LevelID::GamePlay), "Layer_BossArmProjectile", &rightArmDesc, &rightArm);
}

void Boss::BossFire2RightArm::Update(Object* object, _float dt)
{
}

void Boss::BossFire2RightArm::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire2End);
	}
}

void Boss::BossFire2End::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2End));

	XMStoreFloat4(&m_TargetRotation, XMQuaternionRotationRollPitchYaw(0.f, math::ToRadian(180.f), 0.f));
}

void Boss::BossFire2End::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();

	_float4 currRotation = transform->GetQuaternion();
	XMStoreFloat4(&currRotation, XMQuaternionSlerp(XMLoadFloat4(&currRotation), XMLoadFloat4(&m_TargetRotation), dt * 5.f));

	transform->SetQuaternion(currRotation);
}

void Boss::BossFire2End::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (progress>=0.99f)
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossIdle);

		object->GetComponent<TransformComponent>()->SetQuaternion(m_TargetRotation);
	}
}

void Boss::BossFire3Start::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3Start));
}

void Boss::BossFire3Start::Update(Object* object, _float dt)
{
}

void Boss::BossFire3Start::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire4);
	}
}

void Boss::BossFire3InProgress::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3InProgress));
}

void Boss::BossFire3InProgress::Update(Object* object, _float dt)
{
}

void Boss::BossFire3InProgress::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire3End);
	}
}

void Boss::BossFire3End::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3End));
	animator->SetPlaySpeedScale(1.2f);
}

void Boss::BossFire3End::Update(Object* object, _float dt)
{
}

void Boss::BossFire3End::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossIdle);
	}
}

void Boss::BossFire4::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire4));
	animator->SetPlaySpeedScale(0.6f);
}

void Boss::BossFire4::Update(Object* object, _float dt)
{
}

void Boss::BossFire4::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (animator->IsFinished())
	{
		auto boss = static_cast<Boss*>(object);
		boss->ChangeState(&boss->m_BossFire3End);
	}
}

void Boss::BossDie::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Die),false,true);
}

void Boss::BossDie::Update(Object* object, _float dt)
{
}

void Boss::BossDie::TestForExit(Object* object)
{
}
