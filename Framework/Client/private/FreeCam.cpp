#include "pch.h"
#include "FreeCam.h"
#include "EngineCore.h"

//component
#include "CameraComponent.h"
#include "AudioListener.h"


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

	m_strInstanceTag = "FreeCamera";

	AddComponent<CameraComponent>();
	AddComponent<AudioListener>();

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
	auto engine = EngineCore::GetInstance();

	auto cam = GetComponent<CameraComponent>();
	auto transform = GetComponent<TransformComponent>();

	_vector forward = transform->GetForwardV();
	_vector right = transform->GetRightV();
	_float speed = 100.f;
	_float3 rotation = transform->GetRotation();
	_float2 mouseDelta = engine->GetMouseDelta();

	if (m_ActiveMouse)
	{
		_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
		_float pitch = math::ToRadian(mouseDelta.y * 0.1f);
		rotation.y += yaw;
		rotation.x += pitch;
	
		rotation.x = std::clamp(rotation.x, -math::PI + 0.1f, math::PI - 0.1f);
		transform->SetRotation(rotation);
	}
	
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

	if (engine->IsMousePress(MouseButton::RButton))
		m_ActiveMouse = m_ActiveMouse ? false : true;

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
