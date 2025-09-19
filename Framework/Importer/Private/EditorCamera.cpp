#include "ImporterPCH.h"
#include "EditorCamera.h"

//component
#include "CameraComponent.h"

EditorCamera::EditorCamera()
	:Object()
{
}

EditorCamera::EditorCamera(const EditorCamera& prototype)
	:Object(prototype)
{
}

EditorCamera* EditorCamera::Create()
{
	EditorCamera* Instance = new EditorCamera();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EditorCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EditorCamera";

	AddComponent<CameraComponent>();

	return S_OK;
}

HRESULT EditorCamera::Initialize(InitDESC* arg)
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

void EditorCamera::PriorityUpdate(_float dt)
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
	_float3 rotation = transform->GetRotation();
	_float2 mouseDelta = engine->GetMouseDelta();

	if (engine->IsMouseDown(MouseButton::RButton))
	{
		_float yaw = math::ToRadian(mouseDelta.x * 0.1f);
		_float pitch = math::ToRadian(mouseDelta.y * 0.1f);
		rotation.y += yaw;
		rotation.x += pitch;

		rotation.x = std::clamp(rotation.x, -math::PI + 0.1f, math::PI - 0.1f);
		transform->SetRotation(rotation);
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

	engine->SetViewMatrix(cam->GetViewMatrix());
	engine->SetProjMatrix(cam->GetProjMatrix());
}

void EditorCamera::Update(_float dt)
{
	__super::Update(dt);



}

void EditorCamera::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EditorCamera::Clone(InitDESC* arg)
{
	EditorCamera* Instance = new EditorCamera(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EditorCamera::Free()
{
	__super::Free();
}
