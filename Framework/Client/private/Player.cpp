#include "pch.h"
#include "Player.h"
#include "Model.h"
#include "Skeleton.h"
#include "EngineCore.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

//object
#include "Hand.h"
#include "Socket.h"
#include "PlayerCam.h"
#include "Cameleon.h"

//component
#include "ModelComponent.h"
#include "PlayerAnimController.h"
#include "AnimatorComponent.h"
#include "NavigationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

Player::Player()
	:ContainerObject()
{
}

Player::Player(const Player& prototype)
	:ContainerObject(prototype)
{
}

Player* Player::Create()
{
	Player* Instance = new Player();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Player::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "Player";
	AddComponent<PlayerAnimController>();
	AddComponent<NavigationComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();

	return S_OK;
}

HRESULT Player::Initialize(InitDESC* arg)
{
	CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(Parts::Count);

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	Bounding_Sphere::SPHERE_DESC obbDesc{};
	obbDesc.type = ColliderType::Sphere;
	obbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Player);
	obbDesc.center = _float3{ 0.f,5.f,0.f };
	obbDesc.radius = 10.f;

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&obbDesc);
	engine->RegisterCollider(collider);

	auto animatorController = GetComponent<PlayerAnimController>();
	animatorController->SetHandAnimator(m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<AnimatorComponent>());

	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachTransform();
	nav->AttachRigidBody();
	{
		/*equip first weapon*/
		m_Weapons.resize(ENUM_CLASS(WeaponSlot::Count));

		Object* firstWeapon = nullptr;

		Weapon::WEAPON_DESC cameleonDesc{};
		cameleonDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::RightHandSocket)]->GetComponent<TransformComponent>();
		cameleonDesc.parent = this;
		firstWeapon = engine->ClonePrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Foundry", &cameleonDesc);

		if (firstWeapon)
			m_Weapons[ENUM_CLASS(WeaponSlot::Slot1)] = static_cast<Weapon*>(firstWeapon);

		/*add second weapon*/
		Object* secondWeapon = nullptr;
		Weapon::WEAPON_DESC concealedAmmoDesc{};
		concealedAmmoDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::RightHandSocket)]->GetComponent<TransformComponent>();
		concealedAmmoDesc.parent = this;
		secondWeapon = engine->ClonePrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ConcealedAmmo", &concealedAmmoDesc);

		if (secondWeapon)
			m_Weapons[ENUM_CLASS(WeaponSlot::Slot2)] = static_cast<Weapon*>(secondWeapon);

		/*add third weapon*/
		Object* thirdWeapon = nullptr;
		Weapon::WEAPON_DESC icySpearDesc{};
		icySpearDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::RightHandSocket)]->GetComponent<TransformComponent>();
		icySpearDesc.parent = this;
		thirdWeapon = engine->ClonePrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_PoisonousGhost", &icySpearDesc);

		if (thirdWeapon)
			m_Weapons[ENUM_CLASS(WeaponSlot::Slot3)] = static_cast<Weapon*>(thirdWeapon);

		m_eCurrWeaponSlot = WeaponSlot::Slot1;
		Equip();
	}
	
	ChangeState(&m_PlayerIdle);

	return S_OK;
}

void Player::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Player::Update(_float dt)
{
	KeyInput(dt);
	__super::Update(dt);

	MakeHandState();
}

void Player::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Player::AddRecoil(_float power)
{
	static_cast<Hand*>(m_PartObjects[ENUM_CLASS(Parts::Hand)])->AddRecoil(power);
}

_float3 Player::GetAimPosition()
{
	return static_cast<PlayerCam*>(m_PartObjects[ENUM_CLASS(Parts::PlayerCam)])->GetAimPosition();
}

Object* Player::Clone(InitDESC* arg)
{
	Player* Instance = new Player(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Player::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());	

	for (auto& weapon : m_Weapons)
		Safe_Release(weapon);

	__super::Free();
}

HRESULT Player::CreatePartObjects()
{
	/*add player cam*/
	{
		PlayerCam::PLAYER_CAM_DESC camDesc{};
		camDesc.parent = this;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_PlayerCam", ENUM_CLASS(Parts::PlayerCam), &camDesc)))
			return E_FAIL;
	}
	/*add hand*/
	{
		Hand::HAND_DESC handDesc{};
		handDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::PlayerCam)]->GetComponent<TransformComponent>();
		handDesc.parent = this;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Hand", ENUM_CLASS(Parts::Hand), &handDesc)))
			return E_FAIL;
	}
	/*add neck socket*/
	{
		Socket::SOCKET_DESC neckSocket{};
		neckSocket.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<TransformComponent>();
		neckSocket.parent = this;
		neckSocket.parentModel = m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<ModelComponent>();
		neckSocket.boneIndex = m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<ModelComponent>()->GetBoneIndex("Bip001 Neck");
		neckSocket.useScale = true;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::RightHandSocket), &neckSocket)))
			return E_FAIL;
	}

	return S_OK;
}

void Player::Equip()
{
	/*check first*/
	Safe_Release(m_PartObjects[ENUM_CLASS(Parts::Weapon)]);

	Weapon* equipWeapon = m_Weapons[ENUM_CLASS(m_eCurrWeaponSlot)];

	equipWeapon->Idle();
	m_PartObjects[ENUM_CLASS(Parts::Weapon)] = equipWeapon;
	m_PartObjects[ENUM_CLASS(Parts::Weapon)]->AddRef();

	auto animatorController = GetComponent<PlayerAnimController>();
	animatorController->SetWeaponAnimator(equipWeapon->GetComponent<AnimatorComponent>());
}

void Player::KeyInput(_float dt)
{
	/*마우스 보간 넣어야함*/
	auto engine = EngineCore::GetInstance();
	auto hand = static_cast<Hand*>(m_PartObjects[ENUM_CLASS(Parts::Hand)]);

	/*direction controll*/
	{

		_float2 mouseDelta = engine->GetMouseDelta();
		_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
		if (std::abs(yaw) > 0.f)
		{
			m_pTransform->Turn(0.f, yaw);
			hand->SetSwayTargetRoation(-yaw);
		}
	}

	/*weapon controll*/
	{
		if (engine->IsKeyPressed('1'))
		{
			m_eCurrWeaponSlot = WeaponSlot::Slot1;
			ChangeState(&m_PlayerStartEquip);
		}

		if (engine->IsKeyPressed('2'))
		{
			m_eCurrWeaponSlot = WeaponSlot::Slot2;
			ChangeState(&m_PlayerStartEquip);
		}

		if (engine->IsKeyPressed('3'))
		{
			m_eCurrWeaponSlot = WeaponSlot::Slot3;
			ChangeState(&m_PlayerStartEquip);
		}

		if (engine->IsKeyPressed('R'))
			m_Weapons[ENUM_CLASS(m_eCurrWeaponSlot)]->Reload();

		if (engine->IsMouseDown(MouseButton::LButton))
			m_Weapons[ENUM_CLASS(m_eCurrWeaponSlot)]->Fire();

		if (engine->IsMousePress(MouseButton::RButton))
			m_Weapons[ENUM_CLASS(m_eCurrWeaponSlot)]->Skill();

		if (engine->IsMouseAway(MouseButton::LButton))
			m_Weapons[ENUM_CLASS(m_eCurrWeaponSlot)]->Idle();
	}

	/*move controll*/
	{
		auto rigid = GetComponent<RigidBodyComponent>();
		auto nav = GetComponent<NavigationComponent>();

		if (!m_IsDash)
		{
			_vector forward = m_pTransform->GetForwardV();
			_vector right = m_pTransform->GetRightV();
			_float speed = 80.f;
			m_IsWalk = false;


			_vector dir = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			_float3 originVelocity = rigid->GetVelocity();
			_float3 velocity{};

			if (engine->IsKeyDown('W'))
			{
				dir += forward;
				m_IsWalk = true;
			}
	
			if (engine->IsKeyDown('S'))
			{
				dir -= forward;
				m_IsWalk = true;
			}
	
			if (engine->IsKeyDown('D'))
			{
				dir += right;
				m_IsWalk = true;
			}
	
			if (engine->IsKeyDown('A'))
			{
				dir -= right;
				m_IsWalk = true;
			}

			if (engine->IsKeyPressed(VK_SHIFT))
				ChangeState(&m_PlayerDash);

			XMStoreFloat3(&velocity, XMVector3Normalize(dir) * speed);
			velocity.y = originVelocity.y;
			GetComponent<RigidBodyComponent>()->SetVelocity(velocity);

			if (!m_IsJump)
			{
				if (engine->IsKeyDown(VK_SPACE))
				{
					rigid->SetGround(false);
					rigid->AddForce(_float3{ 0.f,90.f,0.f });
					m_IsJump = true;

					hand->StartJump();
				}
			}
			else
			{
				if (rigid->IsGround())
				{
					m_IsJump = false;

					hand->EndJump();
				}
				else
				{
					_float3 velocity = rigid->GetVelocity();
				}
			}

		}

		nav->MoveByVelocity(dt);
	}
}

void Player::MakeHandState()
{
	auto hand = static_cast<Hand*>(m_PartObjects[ENUM_CLASS(Parts::Hand)]);

	if (m_IsWalk)
		hand->Walk();
	else
		hand->Idle();
}

void Player::PlayerIdle::Enter(Object* object)
{
	auto player = static_cast<Player*>(object);
	static_cast<Hand*>(player->m_PartObjects[ENUM_CLASS(Parts::Hand)])->SwayEnable();
}

void Player::PlayerDash::Enter(Object* object)
{
	m_fElapsedTime = 0.f;

	auto player = static_cast<Player*>(object);
	if (player->m_IsWalk)
	{
		auto rigidBody = object->GetComponent<RigidBodyComponent>();
		_float3 velocity = rigidBody->GetVelocity();
		velocity.y = 0.f;
		XMStoreFloat3(&m_DashDirection, XMVector3Normalize(XMLoadFloat3(&velocity)));
	}
	else
	{
		auto transform = object->GetComponent<TransformComponent>();
		m_DashDirection = transform->GetForward();
	}

	player->m_IsWalk = false;
	player->m_IsDash = true;
}

void Player::PlayerDash::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		auto rigidBody = object->GetComponent<RigidBodyComponent>();
		_float3 velocity{};
		if (t < 0.8f)
		{
			XMStoreFloat3(&velocity, m_fStartSpeed * XMLoadFloat3(&m_DashDirection));
			velocity.y = rigidBody->GetVelocity().y;
		}
		else
		{
			t = (t - 0.8f) / 0.2f;

			_float speed = math::Lerp(m_fStartSpeed, m_fEndSpeed, t);
			XMStoreFloat3(&velocity, m_fStartSpeed * XMLoadFloat3(&m_DashDirection));
			velocity.y = rigidBody->GetVelocity().y;
		}

		rigidBody->SetVelocity(velocity);
	}
}

void Player::PlayerDash::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto player = static_cast<Player*>(object);
		player->ChangeState(&player->m_PlayerIdle);

		player->m_IsDash = false;
	}
}

void Player::PlayerStartEquip::Enter(Object* object)
{
	m_fElapsedTime = 0.f;

	auto player = static_cast<Player*>(object);
	static_cast<Hand*>(player->m_PartObjects[ENUM_CLASS(Parts::Hand)])->SwayDisable();
}

void Player::PlayerStartEquip::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	_float 	t = m_fElapsedTime / m_fDuration;
	t = math::EaseOutQuint(t);

	_vector q0, q1;
	_float4 result;
	q0 = XMQuaternionRotationRollPitchYaw(math::ToRadian(0.f), 0.f, 0.f);
	q1 = XMQuaternionRotationRollPitchYaw(45.f, 0.f, 0.f);

	XMStoreFloat4(&result, XMQuaternionSlerp(q0, q1, t));
	
	auto player = static_cast<Player*>(object);
	auto handTransform = player->m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<TransformComponent>();

	handTransform->SetQuaternion(result);
}

void Player::PlayerStartEquip::TestForExit(Object* object)
{
	auto player = static_cast<Player*>(object);

	if (m_fElapsedTime >= m_fDuration)
	{
		player->ChangeState(&player->m_PlayerEndEquip);
	}
}

void Player::PlayerEndEquip::Enter(Object* object)
{
	m_fElapsedTime = 0.f;

	auto player = static_cast<Player*>(object);
	player->Equip();
}

void Player::PlayerEndEquip::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	_float 	t = m_fElapsedTime / m_fDuration;
	t = math::EaseOutQuint(t);

	_vector q0, q1;
	_float4 result;
	q0 = XMQuaternionRotationRollPitchYaw(math::ToRadian(45.f), 0.f, 0.f);
	q1 = XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f);

	XMStoreFloat4(&result, XMQuaternionSlerp(q0, q1, t));

	auto player = static_cast<Player*>(object);
	auto handTransform = player->m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<TransformComponent>();

	handTransform->SetQuaternion(result);
}

void Player::PlayerEndEquip::TestForExit(Object* object)
{
	auto player = static_cast<Player*>(object);

	if (m_fElapsedTime >= m_fDuration)
	{
		auto handTransform = player->m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<TransformComponent>();
		_float4 quaternion{};
		XMStoreFloat4(&quaternion, XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f));
		handTransform->SetQuaternion(quaternion);

		player->ChangeState(&player->m_PlayerIdle);
	}
}


