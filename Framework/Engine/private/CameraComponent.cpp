#include "EnginePCH.h"
#include "CameraComponent.h"
#include "Object.h"
#include "TransformComponent.h"

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

_float4x4 CameraComponent::GetViewMatrix() const
{
	return m_pTransform->GetWorldMatrixInverse();
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

void CameraComponent::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}