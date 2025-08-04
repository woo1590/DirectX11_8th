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
	auto transform = owner->GetComponent<TransformComponent>();

	if (!transform)
		return E_FAIL;

	target = transform;

	return S_OK;
}

void CameraComponent::Free()
{
	__super::Free();
}
