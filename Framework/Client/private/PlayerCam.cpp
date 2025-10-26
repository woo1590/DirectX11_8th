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

	auto engine = EngineCore::GetInstance();

	auto cam = GetComponent<CameraComponent>();
	if (FAILED(cam->Initialize(&camDesc)))
		return E_FAIL;

	m_pTransform->SetPosition(_float3{ 0.f,13.f,0.f });

	engine->AddCamera("PlayerCamera", cam);
	engine->SetMainCamera("PlayerCamera");

	return S_OK;
}

void PlayerCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PlayerCam::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();

	_float2 mouseDelta = engine->GetMouseDelta();
	_float pitch = math::ToRadian(mouseDelta.y * 0.1f);

	m_pTransform->Turn(pitch, 0.f);

	_float4x4 worldMat = m_pTransform->GetWorldMatrix();
	_vector camPosition = XMVectorSet(worldMat._41, worldMat._42, worldMat._43, 1.f);
	_vector forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	forward = XMVector3Normalize(XMVector3TransformNormal(forward, XMLoadFloat4x4(&worldMat)));

	RAY worldRay{};
	RAYCAST_DATA data{};
	XMStoreFloat3(&worldRay.origin, camPosition);
	XMStoreFloat3(&worldRay.direction, forward);
	data = engine->RayCast(worldRay, ENUM_CLASS(ColliderFilter::Ray));;

	if (data.isHit)
		XMStoreFloat3(&m_AimPosition, camPosition + forward * data.worldDistance);
	else
		XMStoreFloat3(&m_AimPosition, camPosition + forward * 10000.f);
}

void PlayerCam::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
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
