#include "EnginePCH.h"
#include "ModelComponent.h"
#include "EngineCore.h"
#include "Model.h"
#include "Material.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Object.h"
#include "MaterialInstance.h"

//component
#include "AnimatorComponent.h"

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
	m_pMateriaInstance = MaterialInstance::Create();
	if (!m_pMateriaInstance)
		return E_FAIL;

	return S_OK;
}

void ModelComponent::Update(_float dt)
{
	__super::Update(dt);
}

HRESULT ModelComponent::ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies)
{
	if (!m_pModel)
		return S_OK;
	
	const auto& meshes = m_pModel->GetBuffers();
	const auto& materials = m_pModel->GetMaterials();

	for (_uint i = 0; i < meshes.size(); ++i)
	{
		RenderProxy proxy{};
		proxy.buffer = meshes[i];
		proxy.material = materials[meshes[i]->GetMaterialIndex()];
		proxy.materialInstance = m_pMateriaInstance;
		proxy.worldMatrix = transform->GetWorldMatrix();

		if (m_pModel->IsSkinned())
		{
			auto animator = m_pOwner->GetComponent<AnimatorComponent>();

			const auto& combinedMatrices = animator->GetCombinedMatrices();
			meshes[i]->ComputeBonePalette(combinedMatrices, m_BonePalettes[i].boneMatrices);

			proxy.numBones = m_BonePalettes[i].boneMatrices.size();
			proxy.boneMatrices = m_BonePalettes[i].boneMatrices.data();
		}

		proxies.push_back(proxy);
	}

	return S_OK;
}

HRESULT ModelComponent::ConnectAnimator()
{
	auto animator = m_pOwner->GetComponent<AnimatorComponent>();
	if (!animator)
	{
		MSG_BOX("Animator is not available");
		return E_FAIL;
	}

	animator->SetSkeleton(m_pModel->GetSkeleton());

	return S_OK;
}

HRESULT ModelComponent::SetModel(_uint levelID, const _string& key)
{
	auto model = EngineCore::GetInstance()->GetModel(levelID, key);
	if (!model)
		return E_FAIL;

	m_pModel = model;
	m_pModel->AddRef();

	m_BonePalettes.resize(m_pModel->GetBuffers().size());
	
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

_int ModelComponent::GetBoneIndex(const _string& boneTag)
{
	auto skeleton = m_pModel->GetSkeleton();

	return skeleton->GetBoneIndexByName(boneTag);
}

_float4x4 ModelComponent::GetOffsetMatrixByIndex(_uint index)
{
	auto skeleton = m_pModel->GetSkeleton();

	return skeleton->GetOffsetMatrixByIndex(index);
}

_float4x4 ModelComponent::GetCombinedMatrixByIndex(_uint index)
{
	auto animator = m_pOwner->GetComponent<AnimatorComponent>();

	if (animator)
	{
		auto& combinedMatrices = animator->GetCombinedMatrices();
		return combinedMatrices[index];
	}
	else
	{
		_float4x4 boneMatrix;
		XMStoreFloat4x4(&boneMatrix, XMMatrixIdentity());
		
		return boneMatrix;
	}
}

void ModelComponent::Free()
{
	__super::Free();

	Safe_Release(m_pMateriaInstance);
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
