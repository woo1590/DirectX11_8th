#include "pch.h"
#include "DropWeapon.h"
#include "Bounding_Sphere.h"

//component
#include "PlayerInteractionComponent.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"

DropWeapon::DropWeapon()
	:Item()
{
}

DropWeapon::DropWeapon(const DropWeapon& prototype)
	:Item(prototype)
{
}

DropWeapon* DropWeapon::Create()
{
	DropWeapon* Instance = new DropWeapon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DropWeapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<ModelComponent>();

	m_strInstanceTag = "DropWeapon";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT DropWeapon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	DROP_WEAPON_DESC* desc = static_cast<DROP_WEAPON_DESC*>(arg);
	m_eWeaponID = desc->weaponID;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Item);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 5.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	switch (m_eWeaponID)
	{
	case Client::WeaponID::Foundry:
	{
		m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_Foundry");
	}break;
	case Client::WeaponID::ConcealedAmmo:
	{
		m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_ConcealedAmmo");
	}break;
	case Client::WeaponID::PoisionousGhost:
	{
		m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_PoisonousGhost");
	}break;
	case Client::WeaponID::Cameleon:
	{
		m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_Cameleon");
	}break;
	case Client::WeaponID::IcySpear:
	{

	}break;
	case Client::WeaponID::Prism:
	{
		m_pTransform->Turn(math::ToRadian(20.f), 0.f);
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_Prism");
	}break;
	default:
		break;
	}

	ChangeState(&m_DropWeaponSpawn);

	return S_OK;
}

void DropWeapon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DropWeapon::Update(_float dt)
{
	__super::Update(dt);

	m_pTransform->Turn(0.f, dt);
}

void DropWeapon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void DropWeapon::Interaction(PlayerInteractionComponent* interaction)
{
	if (&m_DropWeaponIdle == m_CurrState)
	{
		auto engine = EngineCore::GetInstance();

		if (engine->IsKeyPressed('F'))
		{
			interaction->EquipWeapon(m_eWeaponID);
			SetDead();
		}
	}
}

Object* DropWeapon::Clone(InitDESC* arg)
{
	DropWeapon* Instance = new DropWeapon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DropWeapon::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void DropWeapon::DropWeaponSpawn::Enter(Object* object)
{
	auto transform = object->GetComponent<TransformComponent>();

	m_StartPosition = transform->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y += 4.f;
	m_fElapsedTime = 0.f;
}

void DropWeapon::DropWeaponSpawn::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutQuint(t);

		_float3 position{};
		XMStoreFloat3(&position, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPosition), t));

		object->GetComponent<TransformComponent>()->SetPosition(position);
	}
}

void DropWeapon::DropWeaponSpawn::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto dropWeapon = static_cast<DropWeapon*>(object);
		dropWeapon->ChangeState(&dropWeapon->m_DropWeaponIdle);
	}
}

void DropWeapon::DropWeaponIdel::Enter(Object* object)
{
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
}

void DropWeapon::DropWeaponIdel::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	auto transform = object->GetComponent<TransformComponent>();

	_float3 position = m_StartPosition;
	position.y += std::sinf(m_fElapsedTime);

	transform->SetPosition(position);
	transform->Turn(0.f, dt);
}

void DropWeapon::DropWeaponIdel::TestForExit(Object* object)
{
}
