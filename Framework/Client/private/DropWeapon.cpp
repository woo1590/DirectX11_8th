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
	sphereDesc.radius = 3.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	switch (m_eWeaponID)
	{
	case Client::WeaponID::Foundry:
	{
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_Foundry");
	}break;
	case Client::WeaponID::ConcealedAmmo:
	{
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_ConcealedAmmo");
	}break;
	case Client::WeaponID::PoisionousGhost:
	{
		m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_PoisonousGhost");
	}break;
	case Client::WeaponID::Cameleon:
	{

	}break;
	case Client::WeaponID::IcySpear:
	{

	}break;
	case Client::WeaponID::Prism:
	{
		model->SetModel(ENUM_CLASS(LevelID::Static), "Model_DropWeapon_Prism");
	}break;
	default:
		break;
	}

	m_StartPosition = m_pTransform->GetPosition();

	return S_OK;
}

void DropWeapon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DropWeapon::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt * 2.f;
	_float3 position = m_StartPosition;
	position.y += std::sinf(m_fElapsedTime);

	m_pTransform->Turn(0.f, math::ToRadian(dt * 10.f));
	m_pTransform->SetPosition(position);
	
}

void DropWeapon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void DropWeapon::Interaction(PlayerInteractionComponent* interaction)
{
	auto engine = EngineCore::GetInstance();

	if (engine->IsKeyPressed('F'))
	{
		interaction->EquipWeapon(m_eWeaponID);
		SetDead();
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
