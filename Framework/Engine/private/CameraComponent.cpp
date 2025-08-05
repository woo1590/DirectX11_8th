#include "EnginePCH.h"
#include "CameraComponent.h"
#include "Object.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent(Object* owner)
	:Component(owner)
{
}

CameraComponent* CameraComponent::Create(Object* owner)
{
	CameraComponent* Instance = new CameraComponent(owner);

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CameraComponent::Initialize()
{
	return S_OK;
}

void CameraComponent::Free()
{
	__super::Free();
}

_float4x4 CameraComponent::GetViewMatrix() const
{
	return _float4x4{};
}

_float4x4 CameraComponent::GetProjMatrix() const
{
	_float4x4 proj{};
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ));

	return proj;
}
