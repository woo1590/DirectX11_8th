#include "EnginePCH.h"
#include "ModelComponent.h"

ModelComponent::ModelComponent(Object* owner)
	:Component(owner)
{
}

ModelComponent* ModelComponent::Create(Object* owner)
{
	ModelComponent* Instance = new ModelComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ModelComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT ModelComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void ModelComponent::Free()
{
	__super::Free();
}
