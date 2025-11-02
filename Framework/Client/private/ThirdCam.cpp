#include "pch.h"
#include "ThirdCam.h"
#include "EngineCore.h"

//component
#include "CameraComponent.h"
#include "AudioListener.h"


ThirdCam::ThirdCam()
	:Object()
{
}

ThirdCam::ThirdCam(const ThirdCam& prototype)
	:Object(prototype)
{
}

ThirdCam* ThirdCam::Create()
{
	ThirdCam* Instance = new ThirdCam();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ThirdCam::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "FreeCamera";

	AddComponent<CameraComponent>();
	AddComponent<AudioListener>();

	return S_OK;
}

HRESULT ThirdCam::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CameraComponent::CameraDESC camDesc{};
	camDesc.aspect = static_cast<_float>(WinSizeX) / WinSizeY;
	camDesc.fov = XMConvertToRadians(60.f);
	camDesc.nearZ = 1.f;
	camDesc.farZ = 1000.f;

	auto cam = GetComponent<CameraComponent>();
	if (FAILED(cam->Initialize(&camDesc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	m_pTarget = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	if (!m_pTarget)
		return E_FAIL;

	engine->AddCamera("ThirdCamera", cam);

	return S_OK;
}

void ThirdCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

}

void ThirdCam::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();
	_float2 mouseDelta = engine->GetMouseDelta();
	_float pitch = mouseDelta.y * 0.1f;

	_float3 targetDir = m_pTarget->GetComponent<TransformComponent>()->GetForward();
	_float3 dir{};
	_float3 pivot = m_pTarget->GetComponent<TransformComponent>()->GetPosition();
	_float3 targetPosition{};
	_float3 position = m_pTransform->GetPosition();

	XMStoreFloat3(&targetPosition, -1.f * XMLoadFloat3(&targetDir) * 50.f + XMLoadFloat3(&pivot) + XMLoadFloat3(&m_Offset));
	XMStoreFloat3(&position, XMVectorLerp(XMLoadFloat3(&position), XMLoadFloat3(&targetPosition), dt*10.f));

	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&pivot) - XMLoadFloat3(&position)));

	m_pTransform->SetPosition(position);
	m_pTransform->SetForward(dir);
}

void ThirdCam::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* ThirdCam::Clone(InitDESC* arg)
{
	ThirdCam* Instance = new ThirdCam(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void ThirdCam::Free()
{
	__super::Free();
}
