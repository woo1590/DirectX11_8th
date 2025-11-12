#include "pch.h"
#include "ShadowCam.h"
#include "EngineCore.h"

//component
#include "CameraComponent.h"
#include "AudioListener.h"


ShadowCam::ShadowCam()
	:Object()
{
}

ShadowCam::ShadowCam(const ShadowCam& prototype)
	:Object(prototype)
{
}

ShadowCam* ShadowCam::Create()
{
	ShadowCam* Instance = new ShadowCam();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ShadowCam::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "ShadowCam";

	AddComponent<CameraComponent>();
	AddComponent<AudioListener>();

	return S_OK;
}

HRESULT ShadowCam::Initialize(InitDESC* arg)
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

	engine->AddCamera("ShadowCam", cam);
	m_pTransform->Rotate(_float3{ math::ToRadian(80.f),0.f,0.f });
	

	return S_OK;
}

void ShadowCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

}

void ShadowCam::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();
	_float3 targetPosition = m_pTarget->GetComponent<TransformComponent>()->GetPosition();
	_float3 dir = m_pTransform->GetForward();
}

void ShadowCam::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* ShadowCam::Clone(InitDESC* arg)
{
	ShadowCam* Instance = new ShadowCam(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void ShadowCam::Free()
{
	__super::Free();
}
