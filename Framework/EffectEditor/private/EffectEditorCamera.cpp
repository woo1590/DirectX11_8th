#include "pch.h"
#include "EffectEditorCamera.h"

//component
#include "CameraComponent.h"

EffectEditorCamera::EffectEditorCamera()
	:Object()
{
}

EffectEditorCamera::EffectEditorCamera(const EffectEditorCamera& prototype)
	:Object(prototype)
{
}

EffectEditorCamera* EffectEditorCamera::Create()
{
	EffectEditorCamera* Instance = new EffectEditorCamera();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);


	return Instance;
}

HRESULT EffectEditorCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EffectEditorCamera";

	AddComponent<CameraComponent>();

	return S_OK;
}

HRESULT EffectEditorCamera::Initialize(InitDESC* arg)
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

	m_pTransform->SetPosition(_float3(0.f, 100.f, 0.f));
	m_pTransform->Rotate(_float3(math::ToRadian(45.f), 0.f, 0.f));

	auto engine = EngineCore::GetInstance();
	engine->AddCamera("EditorCamera", GetComponent<CameraComponent>());
	engine->SetMainCamera("EditorCamera");

	return S_OK;
}

void EffectEditorCamera::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

	ImGuiIO& io = ImGui::GetIO();
	m_ActiveKeyboard = io.WantCaptureKeyboard || io.WantTextInput ? false : true;

	auto engine = EngineCore::GetInstance();

	auto cam = GetComponent<CameraComponent>();
	auto transform = GetComponent<TransformComponent>();

	_vector forward = transform->GetForwardV();
	_vector right = transform->GetRightV();
	_float speed = 100.f;
	_float2 mouseDelta = engine->GetMouseDelta();

	if (engine->IsKeyAway('Q'))
	{
		if (engine->IsMouseDown(MouseButton::RButton))
		{
			_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
			_float pitch = math::ToRadian(mouseDelta.y * 0.1f);

			m_fTargetPitchYaw.x += pitch;
			m_fTargetPitchYaw.y += yaw;
		}
	}

	if (m_ActiveKeyboard)
	{
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
}

void EffectEditorCamera::Update(_float dt)
{
	__super::Update(dt);

	auto shortestDelta = [](float curr, float target) {
		float d = fmodf(target - curr, XM_2PI);
		if (d > XM_PI)  d -= XM_2PI;
		if (d < -XM_PI) d += XM_2PI;
		return d;
		};

	_float t = 1.f - std::powf(2.f, -dt / 0.06);

	m_fCurrentPitchYaw.x += shortestDelta(m_fCurrentPitchYaw.x, m_fTargetPitchYaw.x) * t;
	m_fCurrentPitchYaw.y += shortestDelta(m_fCurrentPitchYaw.y, m_fTargetPitchYaw.y) * t;

	m_fCurrentPitchYaw.x = std::clamp(m_fCurrentPitchYaw.x, math::ToRadian(-89.f), math::ToRadian(89.f));

	m_pTransform->Rotate(_float3(m_fCurrentPitchYaw.x, m_fCurrentPitchYaw.y, 0.f));
}

void EffectEditorCamera::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EffectEditorCamera::Clone(InitDESC* arg)
{
	EffectEditorCamera* Instance = new EffectEditorCamera(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectEditorCamera::Free()
{
	__super::Free();
}
