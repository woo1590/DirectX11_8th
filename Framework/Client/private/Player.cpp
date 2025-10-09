#include "pch.h"
#include "Player.h"
#include "Model.h"
#include "Skeleton.h"
#include "EngineCore.h"

//object
#include "Hand.h"
#include "Socket.h"
#include "PlayerCam.h"
#include "Cameleon.h"

//component
#include "ModelComponent.h"
#include "PlayerAnimController.h"
#include "AnimatorComponent.h"

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

	auto animatorController = GetComponent<PlayerAnimController>();
	animatorController->SetHandAnimator(m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<AnimatorComponent>());

	/*equip first weapon*/
	m_Weapons.resize(ENUM_CLASS(WeaponSlot::Count));
	auto engine = EngineCore::GetInstance();

	Object* firstWeapon = nullptr;

	Weapon::WEAPON_DESC cameleonDesc{};
	cameleonDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::RightHandSocket)]->GetComponent<TransformComponent>();
	cameleonDesc.parent = this;
	firstWeapon = engine->ClonePrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Cameleon", &cameleonDesc);

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
	thirdWeapon = engine->ClonePrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_IcySpear",&icySpearDesc);

	if (thirdWeapon)
		m_Weapons[ENUM_CLASS(WeaponSlot::Slot3)] = static_cast<Weapon*>(thirdWeapon);
	
	m_eCurrWeaponSlot = WeaponSlot::Slot1;
	Equip();
	ChangeState(&m_PlayerIdle);

	return S_OK;
}

void Player::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Player::Update(_float dt)
{
	__super::Update(dt);
	KeyInput(dt);

}

void Player::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Player::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	return __super::ExtractRenderProxies(proxies);
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
	__super::Free();
	
	for (auto& weapon : m_Weapons)
		Safe_Release(weapon);
}

HRESULT Player::CreatePartObjects()
{
	Hand::HAND_DESC handDesc{};
	handDesc.parent = this;
	if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Hand", ENUM_CLASS(Parts::Hand), &handDesc)))
		return E_FAIL;

	PlayerCam::PLAYER_CAM_DESC camDesc{};
	camDesc.parent = this;
	if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_PlayerCam", ENUM_CLASS(Parts::PlayerCam), &camDesc)))
		return E_FAIL;

	/*add right hand socket*/
	{
		Socket::SOCKET_DESC rightHandSocketDesc{};
		rightHandSocketDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<TransformComponent>();
		rightHandSocketDesc.parent = this;
		rightHandSocketDesc.parentModel = m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<ModelComponent>();
		rightHandSocketDesc.boneIndex = m_PartObjects[ENUM_CLASS(Parts::Hand)]->GetComponent<ModelComponent>()->GetBoneIndex("Bip001 R Hand");
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::RightHandSocket), &rightHandSocketDesc)))
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

	_vector forward = m_pTransform->GetForwardV();
	_vector right = m_pTransform->GetRightV();
	_float speed = 100.f;
	_float2 mouseDelta = engine->GetMouseDelta();

	_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
	_float pitch = math::ToRadian(mouseDelta.y * 0.1f);

	m_pTransform->Turn(pitch, yaw);
	if (engine->IsKeyDown('W'))
		m_pTransform->Translate(forward * speed * dt);
	
	if (engine->IsKeyDown('S'))
		m_pTransform->Translate(-forward * speed * dt);
	
	if (engine->IsKeyDown('D'))
		m_pTransform->Translate(right * speed * dt);
	
	if (engine->IsKeyDown('A'))
		m_pTransform->Translate(-right * speed * dt);
	
	if (engine->IsKeyDown(VK_SPACE))
		m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f) * 100.f * dt);
	
	if (engine->IsKeyDown(VK_SHIFT))
		m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * 100.f * dt);


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

void Player::PlayerStartEquip::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
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
