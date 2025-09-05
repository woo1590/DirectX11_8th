#include "EnginePCH.h"
#include "ModelComponent.h"
#include "Model.h"

ModelComponent::ModelComponent(Object* owner)
	:Component(owner)
{
}

ModelComponent::ModelComponent(const ModelComponent& prototype)
	:Component(prototype)
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


#ifdef USE_IMGUI

void ModelComponent::RenderInspector()
{
	ImGui::PushID(this);

	ImGui::SeparatorText("Model");

	ImGui::PopID();
}

#endif

HRESULT ModelComponent::Bind()
{
	
	return S_OK;
}

Component* ModelComponent::Clone()
{
	return new ModelComponent(*this);
}
