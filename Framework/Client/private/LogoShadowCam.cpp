#include "pch.h"
#include "LogoShadowCam.h"

//component
#include "CameraComponent.h"

LogoShadowCam::LogoShadowCam()
	:Object()
{
}

LogoShadowCam::LogoShadowCam(const LogoShadowCam& prototype)
	:Object(prototype)
{
}

LogoShadowCam* LogoShadowCam::Create()
{
	LogoShadowCam* Instance = new LogoShadowCam();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LogoShadowCam::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<CameraComponent>();
	m_strInstanceTag = "ShadowCam";

	return S_OK;
}

HRESULT LogoShadowCam::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*camera*/
	CameraComponent::CameraDESC camDesc{};
	camDesc.aspect = static_cast<_float>(WinSizeX) / WinSizeY;
	camDesc.fov = XMConvertToRadians(60.f);
	camDesc.nearZ = 1.f;
	camDesc.farZ = 2000.f;

	auto cam = GetComponent<CameraComponent>();
	if (FAILED(cam->Initialize(&camDesc)))
		return E_FAIL;

	engine->AddCamera("LogoShadowCam", cam);
	engine->SetShadowCamera("LogoShadowCam");

	m_pTransform->SetPosition(_float3{ -180.f,47.f,-167.f });
	m_pTransform->Rotate(_float3{ math::ToRadian(33.f),0.f,0.f });

	return S_OK;
}

void LogoShadowCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LogoShadowCam::Update(_float dt)
{
	__super::Update(dt);
}

void LogoShadowCam::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* LogoShadowCam::Clone(InitDESC* arg)
{
	LogoShadowCam* Instance = new LogoShadowCam(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LogoShadowCam::Free()
{
	EngineCore::GetInstance()->RemoveCamera("LogoShadowCam");

	__super::Free();
}
