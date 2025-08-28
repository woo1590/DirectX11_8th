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

CameraComponent* CameraComponent::Create(Object* pOnwer, InitDESC* arg)
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

		return S_OK;
	}

	return S_OK;
}

void CameraComponent::Free()
{
	__super::Free();
}

_matrix CameraComponent::GetViewMatrix() const
{
	auto transform = m_pOwner->GetComponent<TransformComponent>();

	return transform->GetWorldMatrixInverse();
}

_matrix CameraComponent::GetProjMatrix() const
{
	return XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNearZ, m_fFarZ);
}
