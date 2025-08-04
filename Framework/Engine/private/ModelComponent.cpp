#include "EnginePCH.h"
#include "ModelComponent.h"

ModelComponent::ModelComponent(Object* owner)
	:Component(owner)
{
}

ModelComponent* ModelComponent::Create(Object* owner)
{
	ModelComponent* Instance = new ModelComponent(owner);

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ModelComponent::Initialize()
{
	return S_OK;
}

void ModelComponent::Free()
{
	__super::Free();
}
