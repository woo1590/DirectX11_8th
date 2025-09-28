#include "pch.h"
#include "Player.h"

//object
#include "Hand.h"
#include "Socket.h"
#include "PlayerCam.h"

//component
#include "ModelComponent.h"

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

	return S_OK;
}

void Player::KeyInput(_float dt)
{
	/*마우스 보간 넣어야함*/
	auto engine = EngineCore::GetInstance();

	_vector forward = m_pTransform->GetForwardV();
	_vector right = m_pTransform->GetRightV();
	_float speed = 100.f;
	_float3 rotation = m_pTransform->GetRotation();
	_float2 mouseDelta = engine->GetMouseDelta();

	_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
	_float pitch = math::ToRadian(mouseDelta.y * 0.1f);
	rotation.y += yaw;
	rotation.x += pitch;

	rotation.x = std::clamp(rotation.x, -math::PI + 0.1f, math::PI - 0.1f);
	m_pTransform->SetRotation(rotation);

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
}
