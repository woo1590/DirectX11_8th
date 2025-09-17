#include "EnginePCH.h"
#include "ModelComponent.h"
#include "EngineCore.h"
#include "Model.h"
#include "Material.h"

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

void ModelComponent::Update(_float dt)
{
	m_pModel->Update(dt);
}

HRESULT ModelComponent::ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies)
{
	return m_pModel->ExtractRenderProxy(transform, proxies, m_pOverrideMtrl);
}

HRESULT ModelComponent::SetModel(_uint levelID, const _string& key)
{
	auto model = EngineCore::GetInstance()->GetModel(levelID, key);
	if (!model)
		return E_FAIL;

	m_pModel = model;
	m_pModel->AddRef();

	return S_OK;
}

void ModelComponent::SetOverride(Material* pMaterial)
{
	m_pOverrideMtrl = pMaterial;
	m_pOverrideMtrl->AddRef();
}

void ModelComponent::ClearOverride()
{
	Safe_Release(m_pOverrideMtrl);
}

void ModelComponent::Free()
{
	__super::Free();

	Safe_Release(m_pOverrideMtrl);
	Safe_Release(m_pModel);
}


#ifdef USE_IMGUI

void ModelComponent::RenderInspector()
{
	ImGui::PushID(this);

	ImGui::SeparatorText("Model");

	ImGui::PopID();
}

#endif

Component* ModelComponent::Clone()
{
	return new ModelComponent(*this);
}
