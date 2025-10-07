#include "pch.h"
#include "PlayerCam.h"
#include "CameraComponent.h"

PlayerCam::PlayerCam()
	:PartObject()
{
}

PlayerCam::PlayerCam(const PlayerCam& prototype)
	:PartObject(prototype)
{
}

PlayerCam* PlayerCam::Create()
{
	PlayerCam* Instance = new PlayerCam();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PlayerCam::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<CameraComponent>();

	return S_OK;
}

HRESULT PlayerCam::Initialize(InitDESC* arg)
{
	PLAYER_CAM_DESC* desc = static_cast<PLAYER_CAM_DESC*>(arg);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CameraComponent::CameraDESC camDesc{};
	camDesc.aspect = static_cast<_float>(WinSizeX) / WinSizeY;
	camDesc.fov = math::ToRadian(60.f);
	camDesc.nearZ = 0.1f;
	camDesc.farZ = 500.f;

	if (FAILED(GetComponent<CameraComponent>()->Initialize(&camDesc)))
		return E_FAIL;

	return S_OK;
}

void PlayerCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PlayerCam::Update(_float dt)
{
	__super::Update(dt);
}

void PlayerCam::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	auto engine = EngineCore::GetInstance();
	auto cam = GetComponent<CameraComponent>();

	engine->SetViewMatrix(cam->GetViewMatrix());
	engine->SetProjMatrix(cam->GetProjMatrix());
}

Object* PlayerCam::Clone(InitDESC* arg)
{
	PlayerCam* Instance = new PlayerCam(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PlayerCam::Free()
{
	__super::Free();
}
