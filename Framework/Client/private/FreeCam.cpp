#include "pch.h"
#include "FreeCam.h"

//component
#include "CameraComponent.h"

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

	m_strInstanceTag = "EditorCamera";

	AddComponent<CameraComponent>();

	return S_OK;
}

HRESULT FreeCam::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CameraComponent::CameraDESC camDesc{};
	camDesc.aspect = static_cast<_float>(WinSizeX) / WinSizeY;
	camDesc.fov = math::ToRadian(40.f);
	camDesc.nearZ = 1.f;
	camDesc.farZ = 1000.f;

	if (FAILED(GetComponent<CameraComponent>()->Initialize(&camDesc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	engine->AddCamera("FreeCam", GetComponent<CameraComponent>());
	engine->SetMainCamera("FreeCam");

	m_StartPosition = _float3{ -180.f,15.3f,-110.f };
	m_TargetPosition = _float3{ -190.f, 17.3f, -120.5f };

	m_pTransform->SetPosition(m_StartPosition);
	m_pTransform->SetQuaternion(_float4{ 0.01f,-0.93f,-0.03f,-0.36f });

	return S_OK;
}

void FreeCam::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

	//auto engine = EngineCore::GetInstance();
	//
	//auto cam = GetComponent<CameraComponent>();
	//auto transform = GetComponent<TransformComponent>();
	//
	//_vector forward = transform->GetForwardV();
	//_vector right = transform->GetRightV();
	//_float speed = 100.f;
	//
	//if (engine->IsMouseDown(MouseButton::RButton))
	//{
	//	_float2 mouseDelta = engine->GetMouseDelta();
	//
	//	_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
	//	_float pitch = math::ToRadian(mouseDelta.y * 0.1f);
	//
	//	transform->Turn(pitch, yaw);
	//
	//}
	//
	//if (engine->IsKeyDown('W'))
	//	m_pTransform->Translate(forward * speed * dt);
	//
	//if (engine->IsKeyDown('S'))
	//	m_pTransform->Translate(-forward * speed * dt);
	//
	//if (engine->IsKeyDown('D'))
	//	m_pTransform->Translate(right * speed * dt);
	//
	//if (engine->IsKeyDown('A'))
	//	m_pTransform->Translate(-right * speed * dt);
	//
	//if (engine->IsKeyDown(VK_SPACE))
	//	m_pTransform->Translate(XMVectorSet(0.f, 1.f, 0.f, 0.f) * 100.f * dt);
	//
	//if (engine->IsKeyDown(VK_SHIFT))
	//	m_pTransform->Translate(XMVectorSet(0.f, -1.f, 0.f, 0.f) * 100.f * dt);
}

void FreeCam::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	_float t = m_fElapsedTime / m_fDuration;
	t = math::EaseOutSline(t);

	_float3 currPosition{};
	XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPosition), t));
	m_pTransform->SetPosition(currPosition);

	if (EngineCore::GetInstance()->IsKeyPressed(VK_SPACE))
		m_fElapsedTime = 0.f;
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
	EngineCore::GetInstance()->RemoveCamera("FreeCam");
	__super::Free();
}
