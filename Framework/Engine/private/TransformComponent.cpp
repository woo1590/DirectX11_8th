#include "EnginePCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(Object* owner)
	:Component(owner)
{
}

TransformComponent* TransformComponent::Create(Object* owner)
{
	TransformComponent* Instance = new TransformComponent(owner);

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TransformComponent::Initialize()
{
	return S_OK;
}

void TransformComponent::Free()
{
	__super::Free();
}
