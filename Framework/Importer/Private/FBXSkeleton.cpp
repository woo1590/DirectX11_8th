#include "ImporterPCH.h"
#include "FBXSkeleton.h"
#include "FBXBone.h"

FBXSkeleton::FBXSkeleton()
{
}

FBXSkeleton * FBXSkeleton::Create(const aiScene* pScene)
{
	FBXSkeleton* Instance = new FBXSkeleton();

	if (FAILED(Instance->Initialize(pScene)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXSkeleton::Initialize(const aiScene* pScene)
{
	if (FAILED(ProcessNode(pScene->mRootNode, -1)))
		return E_FAIL;

	m_iNumBones = m_Bones.size();

	return S_OK;
}

void FBXSkeleton::UpdateCombiendTransformation()
{
	for (const auto& bone : m_Bones)
		bone->UpdateCombiendTransformation(m_Bones);
}

void FBXSkeleton::Export(std::ofstream& out)
{
	SKELETON_FORMAT skeletonFormat{};
	skeletonFormat.numBones = m_iNumBones;

	out.write(reinterpret_cast<const char*>(&skeletonFormat), sizeof(SKELETON_FORMAT));

	for (const auto& bone : m_Bones)
		bone->Export(out);
}

_int FBXSkeleton::GetBoneIndexByName(const _string& name)
{
	_int index = 0;
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](FBXBone* bone)
		{
			if (name == bone->GetBoneTag())
				return true;

			++index;
			return false;
		});

	if (iter == m_Bones.end())
		return -1;
	else
		return index;
}

_matrix FBXSkeleton::GetCombinedTransformMatrix(_uint index)
{
	return m_Bones[index]->GetCombinedTransformMatrix();
}

void FBXSkeleton::Free()
{
	__super::Free();

	for (auto& bone : m_Bones)
		Safe_Release(bone);
	m_Bones.clear();
}

#ifdef USE_IMGUI

void FBXSkeleton::RenderInspector()
{
	ImGui::PushID(this);
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_string name = std::to_string(i) + " : " + m_Bones[i]->GetBoneTag();

		ImGui::Text(name.c_str());
	}

	ImGui::PopID();
}

#endif

HRESULT FBXSkeleton::ProcessNode(aiNode* childNode, _int parentIndex)
{
	auto bone = FBXBone::Create(childNode, parentIndex);
	if (!bone)
	{
		MSG_BOX("Failed to create : FBXBone");
		return E_FAIL;
	}

	m_Bones.push_back(bone);

	_int index = m_Bones.size() - 1;

	for (_uint i = 0; i < childNode->mNumChildren; ++i)
	{
		if (FAILED(ProcessNode(childNode->mChildren[i], index)))
			return E_FAIL;
	}

	return S_OK;
}
