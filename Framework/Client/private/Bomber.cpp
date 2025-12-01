#include "pch.h"
#include "Bomber.h"
#include "Bounding_AABB.h"
#include "Random.h"
#include "DamageFont.h"
#include "MaterialInstance.h"
#include "DefaultBullet.h"

#include "Fracture.h"
#include "EnemyHpPanel.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "NavigationComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "LightComponent.h"

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
	AddComponent<LightComponent>();
	AddComponent<StatusComponent>();

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
	aabbDesc.useResolve = true;
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,6.f,0.f };
	aabbDesc.halfSize = _float3{ 4.f,6.f,4.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Enemy_Bomber");
	model->Initialize(nullptr);

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Enemy_Bomber");

	model->ConnectAnimator();

	/*navigation*/
	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachTransform();
	nav->AttachRigidBody();
	nav->SpawnInCell(3);
	nav->SetMoveSpeed(35.f);
	nav->SetArriveRange(60.f);

	/*outline model*/
	m_pOutLineModel = ModelComponent::Create(this);
	m_pOutLineModel->SetModel(ENUM_CLASS(LevelID::Static), "Model_Enemy_Bomber");
	m_pOutLineModel->Initialize(nullptr);
	auto outlineMtrlInstance = m_pOutLineModel->GetMaterialInstance();
	outlineMtrlInstance->SetPass("OutLine_Pass");
	outlineMtrlInstance->SetFloat4("g_OutLineColor", _float4(0.f, 0.f, 0.f, 1.f));
	outlineMtrlInstance->SetFloat("g_OutLineWidth", 0.1f);

	m_iHpPanelBoneIndex = model->GetBoneIndex("MonsterHp");
	//m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });

	/*light*/
	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.color = _float4{ 1.f,0.3f,0.f,1.f };
	lightDesc.type = LightType::Point;
	lightDesc.range = 50.f;
	auto light = GetComponent<LightComponent>();
	light->Initialize(&lightDesc);
	engine->RegisterLight(light);

	/*status*/
	auto status = GetComponent<StatusComponent>();
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.hp = 80;
	statusDesc.attackPower = 0;
	statusDesc.speed = 30.f;
	status->Initialize(&statusDesc);

	return S_OK;
}

HRESULT Bomber::LateInitialize()
{
	__super::LateInitialize();

	ChangeState(&m_BomberShow);

	return S_OK;
}

void Bomber::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Bomber::Update(_float dt)
{
	__super::Update(dt);

	_float4x4 hpPanelMat = GetComponent<ModelComponent>()->GetCombinedMatrixByIndex(m_iHpPanelBoneIndex);
	_float4x4 worldMat = m_pTransform->GetWorldMatrix();
	_matrix panelMat = XMLoadFloat4x4(&hpPanelMat) * XMLoadFloat4x4(&worldMat);
	_vector positionV, scaleV, rotV;
	_float3 position{};
	XMMatrixDecompose(&scaleV, &rotV, &positionV, panelMat);
	XMStoreFloat3(&position, positionV);

	auto engine = EngineCore::GetInstance();
	EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
	param.ownerID = m_iEnemyID;
	param.position = position;

	engine->PublishEvent(ENUM_CLASS(EventID::EnemyHpPanelPositionUpdate), param);
}

void Bomber::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Bomber::OnCollisionEnter(ColliderComponent* otherCollider)
{
	__super::OnCollisionEnter(otherCollider);

	auto engine = EngineCore::GetInstance();
	auto random = engine->GetRandom();

	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case ColliderFilter::PlayerAttack:
	{
		auto status = GetComponent<StatusComponent>();
		auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();


		status->BeAttacked(otherStatus->GetDesc().attackPower);
		if (0 == status->GetDesc().hp && m_CurrState != &m_BomberDead)
			ChangeState(&m_BomberDead);

		DamageFont::DAMAGE_FONT_DESC desc{};
		desc.position = m_pTransform->GetPosition();
		desc.position.y += 5.f;
		desc.fontSize = 0.04f;
		desc.number = random->get<_uint>(600, 900);
		desc.color = _float4{ 1.f,1.f,1.f,1.f };

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DamageFont", engine->GetCurrLevelID(), "Layer_UI", &desc);


		EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
		param.ownerID = m_iEnemyID;
		param.ratio = status->GetHpRatio();
		engine->PublishEvent(ENUM_CLASS(EventID::EnemyHealthDecrease), param);

	}break;
	case ColliderFilter::PlayerProjectile:
	{
		auto status = GetComponent<StatusComponent>();
		auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

		status->BeAttacked(otherStatus->GetDesc().attackPower);
		if (0 == status->GetDesc().hp && m_CurrState != &m_BomberDead)
			ChangeState(&m_BomberDead);

		DamageFont::DAMAGE_FONT_DESC desc{};
		desc.position = m_pTransform->GetPosition();
		desc.position.y += 5.f;
		desc.fontSize = 0.04f;
		desc.number = random->get<_uint>(600, 900);
		desc.color = _float4{ 1.f,1.f,1.f,1.f };

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DamageFont", engine->GetCurrLevelID(), "Layer_UI", &desc);

		EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
		param.ownerID = m_iEnemyID;
		param.ratio = status->GetHpRatio();
		engine->PublishEvent(ENUM_CLASS(EventID::EnemyHealthDecrease), param);

	}break;
	default:
		break;
	}
}

void Bomber::Explode()
{
	m_isDead = true;

	auto engine = EngineCore::GetInstance();

	_float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();

	EffectContainer::EFFECT_CONTAINER_DESC desc{};
	desc.position = GetComponent<TransformComponent>()->GetPosition();
	XMStoreFloat3(&desc.forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&desc.position)));

	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Explode", engine->GetCurrLevelID(), "Layer_Effect", &desc);
	engine->Play3DSound("SFX_DynamiteExplode", GetComponent<TransformComponent>()->GetPosition(), 0.6f);

	EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
	param.ownerID = m_iEnemyID;
	engine->PublishEvent(ENUM_CLASS(EventID::EnemyDead), param);
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
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());
	__super::Free();
}

void Bomber::BomberShow::Enter(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	EffectContainer::EFFECT_CONTAINER_DESC effectDesc{};
	effectDesc.position = object->GetComponent<TransformComponent>()->GetPosition();
	XMStoreFloat3(&effectDesc.forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpawnSmoke", engine->GetCurrLevelID(), "Layer_Effect", &effectDesc);
	
	m_fElapsedTime = 0.f;
}

void Bomber::BomberShow::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
}

void Bomber::BomberShow::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto bomber = static_cast<Bomber*>(object);
		bomber->ChangeState(&bomber->m_BomberIdle);
	}
}

void Bomber::BomberIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);
}

void Bomber::BomberIdle::Update(Object* object, _float dt)
{
}

void Bomber::BomberIdle::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	if (distance < 500.f)
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

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	auto transform = object->GetComponent<TransformComponent>();
	auto nav = object->GetComponent<NavigationComponent>();

	_uint currCellIndex = nav->GetCurrCellIndex();
	_uint targetCellIndex = player->GetComponent<NavigationComponent>()->GetCurrCellIndex();
	_float3 position = transform->GetPosition();
	_float3 targetPosition = player->GetComponent<TransformComponent>()->GetPosition();

	nav->FindPath(position, currCellIndex, targetPosition, targetCellIndex);
	nav->MoveByPath(dt);
}

void Bomber::BomberRun::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	_float3 position = object->GetComponent<TransformComponent>()->GetPosition();
	_float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

	auto bomber = static_cast<Bomber*>(object);
	if (distance >= 500.f)
	{
		bomber->ChangeState(&bomber->m_BomberIdle);
	}
	else if (distance < 20.f)
	{
		bomber->ChangeState(&bomber->m_BomberAttack);
	}
}

void Bomber::BomberAttack::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack1));
}

void Bomber::BomberAttack::Update(Object* object, _float dt)
{
}

void Bomber::BomberAttack::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	if (animator->IsFinished())
	{
		auto bomber = static_cast<Bomber*>(object);
		bomber->Explode();
	}
}

void Bomber::BomberDead::Enter(Object* object)
{
	object->SetDead();

	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();

	_float3 camPosition = engine->GetCameraContext().camPosition;
	_float3 position = transform->GetPosition();
	_float3 hitDir{};
	XMStoreFloat3(&hitDir, XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&camPosition)));

	Fracture::FRACTURE_DESC desc{};
	desc.quaternion = transform->GetQuaternion();

	auto random = engine->GetRandom();
	for (_uint i = 1; i < 11; i += 2)
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

		_string modelTag = "Bomber" + std::to_string(i);
		desc.modelTag = "Model_Fracture_" + modelTag;

		Object* fracture = nullptr;
		desc.spawnNavCell = object->GetComponent<NavigationComponent>()->GetCurrCellIndex();
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Fracture", engine->GetCurrLevelID(), "Layer_Fracture", &desc, &fracture);

		fracture->GetComponent<RigidBodyComponent>()->AddImpulse(force);
		fracture->GetComponent<RigidBodyComponent>()->AddAngularImpulse(angularForce);
	}
}

void Bomber::BomberDead::Update(Object* object, _float dt)
{
}

void Bomber::BomberDead::TestForExit(Object* object)
{
}
