#include "pch.h"
#include "Chest.h"
#include "Bounding_OBB.h"

//object
#include "DropWeapon.h"

//component
#include "ColliderComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Chest::Chest()
	:Object()
{
}

Chest::Chest(const Chest& prototype)
	:Object(prototype)
{
}

Chest* Chest::Create()
{
	Chest* Instance = new Chest();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Chest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();

	return S_OK;
}

HRESULT Chest::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CHEST_DESC* desc = static_cast<CHEST_DESC*>(arg);

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Chest);
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 0.f,3.f,0.f };
	obbDesc.halfSize = _float3{ 6.f,3.f,6.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Chest");

	/*animator*/
	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Chest");

	model->ConnectAnimator();

	m_eSpawnWeaponID = desc->weaponID;
	m_pTransform->SetScale(_float3{ 1.6f,1.6f,1.6f });
	ChangeState(&m_ChestClosed);

	return S_OK;
}

void Chest::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Chest::Update(_float dt)
{
	__super::Update(dt);

	if (EngineCore::GetInstance()->IsKeyPressed('L'))
		ChangeState(&m_ChestOpen);
}

void Chest::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Chest::OnCollisionEnter(ColliderComponent* otherCollider)
{
}

void Chest::OnCollisionStay(ColliderComponent* otherCollider)
{
	if (&m_ChestClosed == m_CurrState)
	{
		auto engine = EngineCore::GetInstance();

		if (engine->IsKeyPressed('F'))
			ChangeState(&m_ChestOpen);
	}
}

Object* Chest::Clone(InitDESC* arg)
{
	Chest* Instance = new Chest(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Chest::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void Chest::ChestClosed::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Closed));
}

void Chest::ChestClosed::Update(Object* object, _float dt)
{
}

void Chest::ChestClosed::TestForExit(Object* object)
{
}

void Chest::ChestOpen::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Open));

	m_IsWeaponSpawned = false;
}

void Chest::ChestOpen::Update(Object* object, _float dt)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	_float progress = animator->GetProgress();

	if (!m_IsWeaponSpawned && progress >= 0.65f)
	{
		auto engine = EngineCore::GetInstance();
		auto chest = static_cast<Chest*>(object);
		_float3 position = chest->GetComponent<TransformComponent>()->GetPosition();
		
		DropWeapon::DROP_WEAPON_DESC dropWeaponDesc{};
		dropWeaponDesc.weaponID = chest->m_eSpawnWeaponID;
		dropWeaponDesc.position = position;
		dropWeaponDesc.position.y += 10.f;

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DropWeapon", engine->GetCurrLevelID(), "Layer_DropWeapon", &dropWeaponDesc);

		m_IsWeaponSpawned = true;
	}
}

void Chest::ChestOpen::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto chest = static_cast<Chest*>(object);
		chest->ChangeState(&chest->m_ChestOpend);
	}
}

void Chest::ChestClose::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Close));
}

void Chest::ChestClose::Update(Object* object, _float dt)
{
}

void Chest::ChestClose::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		auto chest = static_cast<Chest*>(object);
		chest->ChangeState(&chest->m_ChestClosed);
	}
}

void Chest::ChestOpened::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Opened));
}

void Chest::ChestOpened::Update(Object* object, _float dt)
{
}

void Chest::ChestOpened::TestForExit(Object* object)
{
}
