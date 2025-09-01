#include "pch.h"
#include "FreeCam.h"
#include "CameraComponent.h"
#include "EngineCore.h"

FreeCam::FreeCam()
	:Object()
{
}

FreeCam::FreeCam(const FreeCam& prototype)
	:Object(prototype)
{
}

FreeCam* FreeCam::Create()
{
	FreeCam* Instance = new FreeCam();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FreeCam::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<CameraComponent>();

	return S_OK;
}

HRESULT FreeCam::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CameraComponent::CameraDESC camDesc{};
	camDesc.aspect = static_cast<_float>(WinSizeX) / WinSizeY;
	camDesc.fov = XMConvertToRadians(60.f);
	camDesc.nearZ = 1.f;
	camDesc.farZ = 1000.f;

	if (FAILED(GetComponent<CameraComponent>()->Initialize(&camDesc)))
		return E_FAIL;

	return S_OK;
}

void FreeCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
	
	if (GetAsyncKeyState('W'))
	{
		_float3 velocity{ 0.f,0.f,100.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	if (GetAsyncKeyState('A'))
	{
		_float3 velocity{ -100.f,0.f,0.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	if (GetAsyncKeyState('D'))
	{
		_float3 velocity{ 100.f,0.f,0.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	if (GetAsyncKeyState('S'))
	{
		_float3 velocity{ 0.f,0.f,-100.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	auto engine = EngineCore::GetInstance();
	auto cam = GetComponent<CameraComponent>();

	engine->SetViewMatrix(cam->GetViewMatrix());
	engine->SetProjMatrix(cam->GetProjMatrix());
}

void FreeCam::Update(_float dt)
{
	__super::Update(dt);

	

}

void FreeCam::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* FreeCam::Clone(InitDESC* arg)
{
	FreeCam* Instance = new FreeCam(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void FreeCam::Free()
{
	__super::Free();
}
