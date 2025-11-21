#include "EnginePCH.h"
#include "CameraComponent.h"
#include "Object.h"
#include "TransformComponent.h"
#include "EngineCore.h"
#include "Random.h"

CameraComponent::CameraComponent(Object* pOnwer)
	:Component(pOnwer)
{
}

CameraComponent::CameraComponent(const CameraComponent& prototype)
	:Component(prototype),
	m_fAspect(prototype.m_fAspect),
	m_fFov(prototype.m_fFov),
	m_fNearZ(prototype.m_fNearZ),
	m_fFarZ(prototype.m_fFarZ)
{
}

CameraComponent* CameraComponent::Create(Object* pOnwer)
{
	CameraComponent* Instance = new CameraComponent(pOnwer);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CameraComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CameraComponent::Initialize(InitDESC* arg)
{
	if (arg)
	{
		CameraDESC* desc = static_cast<CameraDESC*>(arg);

		m_fAspect = desc->aspect;
		m_fFov = desc->fov;
		m_fNearZ = desc->nearZ;
		m_fFarZ = desc->farZ;
	}

	/*Transform Late Initialize*/
	auto transform = m_pOwner->GetComponent<TransformComponent>();
	if (!transform)
		return E_FAIL;

	m_pTransform = transform;
	m_pTransform->AddRef();

	return S_OK;
}

void CameraComponent::Update(_float dt)
{
	if (m_IsShake)
	{
		m_fShakeElapsedTime += dt;
		if (m_fShakeElapsedTime >= m_fShakeDuration)
		{
			m_fShakeElapsedTime = 0.f;
			m_fShakeDuration = 0.f;
			m_ShakeOffset = _float3{ 0.f,0.f,0.f };
			m_Frequency = _float3{ 0.f,0.f,0.f };
			m_Phase = _float3{ 0.f,0.f,0.f };
			m_fShakePower = 0.f;
			m_IsShake = false;

			return;
		}

		_float t = std::expf(-20.f * m_fShakeElapsedTime);
		//t = math::SmoothStep(t);
		_float3 offset{};
		m_ShakeOffset.x = std::sinf(m_Frequency.x * m_fShakeElapsedTime * math::TWO_PI + m_Phase.x) * t * m_Amp.x;
		m_ShakeOffset.y = std::sinf(m_Frequency.y * m_fShakeElapsedTime * math::TWO_PI + m_Phase.y) * t * m_Amp.y;
		m_ShakeOffset.z = std::sinf(m_Frequency.z * m_fShakeElapsedTime * math::TWO_PI + m_Phase.z) * t * m_Amp.z;
	}
}

_float4x4 CameraComponent::GetViewMatrix() const
{
	//return m_pTransform->GetWorldMatrixInverse();
	_float4x4 viewMatrix{};
	_float4x4 worldMatrix = m_pTransform->GetWorldMatrix();
	_vector positionV, scaleV, rotationV;
	_float3 position{}, scale{};
	XMMatrixDecompose(&scaleV, &rotationV, &positionV, XMLoadFloat4x4(&worldMatrix));

	if (m_IsShake)
	{
		_vector shakeOffset = XMQuaternionRotationRollPitchYaw(m_ShakeOffset.x, m_ShakeOffset.y, m_ShakeOffset.z);
		rotationV = XMQuaternionMultiply(rotationV, shakeOffset);
	}

	XMStoreFloat3(&position, positionV);
	XMStoreFloat3(&scale, scaleV);
	
	XMStoreFloat4x4(&viewMatrix,	XMMatrixScaling(scale.x, scale.y, scale.z) *
									XMMatrixRotationQuaternion(rotationV) *
									XMMatrixTranslation(position.x, position.y, position.z));

	XMStoreFloat4x4(&viewMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&viewMatrix)));

	return viewMatrix;
}

_float4x4 CameraComponent::GetProjMatrix()
{
	if (m_isDirty)
	{
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ));

		m_isDirty = false;
	}

	return m_ProjMatrix;
}

void CameraComponent::MakeShake(_float duration, _float power)
{
	auto random = EngineCore::GetInstance()->GetRandom();

	m_fShakeDuration = duration;
	m_fShakePower = power;
	m_IsShake = true;
	m_fShakeElapsedTime = 0.f;

	m_Amp.x = 0.5f * power;
	m_Amp.y = 0.5f * power;
	m_Amp.z = power;

	m_Frequency.x = 10.f;
	m_Frequency.y = 10.f * 0.8f;
	m_Frequency.z = 10.f * 0.2f;

	m_Phase.x = random->get<_float>(0.f, 1.f) * math::TWO_PI;
	m_Phase.y = random->get<_float>(0.f, 1.f) * math::TWO_PI;
	m_Phase.z = random->get<_float>(0.f, 1.f) * math::TWO_PI;
}

void CameraComponent::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}

#ifdef USE_IMGUI

void CameraComponent::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Camera",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();

		_float fovDgree = XMConvertToDegrees(m_fFov);

		m_isDirty |= ImGui::DragFloat("Fov", &fovDgree, 0.1f, -FLT_MAX, FLT_MAX);
		m_isDirty |= ImGui::DragFloat("Aspect", &m_fAspect, 0.1f, -FLT_MAX, FLT_MAX);
		m_isDirty |= ImGui::DragFloat("NearZ", &m_fNearZ, 0.1f, -FLT_MAX, FLT_MAX);
		m_isDirty |= ImGui::DragFloat("FarZ", &m_fFarZ, 0.1f, -FLT_MAX, FLT_MAX);

		m_fFov = XMConvertToRadians(fovDgree);
	}

	ImGui::PopID();
}

#endif