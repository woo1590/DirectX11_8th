#include "ImporterPCH.h"
#include "FBXBone.h"

FBXBone::FBXBone()
{
}

FBXBone* FBXBone::Create(const aiNode* pNode, _int parentIndex)
{
	FBXBone* Instance = new FBXBone();

	if (FAILED(Instance->Initialize(pNode, parentIndex)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXBone::Initialize(const aiNode* pNode, _int parentIndex)
{
	m_strBoneTag = pNode->mName.C_Str();
	m_iParentIndex = parentIndex;

	memcpy_s(&m_TransformationMatrix, sizeof(_float4x4), &pNode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void FBXBone::UpdateCombiendTransformation(std::vector<FBXBone*>& modelBones)
{
	if (m_iParentIndex == -1)
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * modelBones[m_iParentIndex]->GetCombinedTransformMatrix());
	}
}

HRESULT FBXBone::Export(std::ofstream& out)
{
	BONE_FORMAT boneFormat{};
	strncpy_s(boneFormat.boneTag, MAX_PATH, m_strBoneTag.c_str(), sizeof(boneFormat.boneTag) - 1);
	boneFormat.parentIndex = m_iParentIndex;
	boneFormat.transformationMatrix = m_TransformationMatrix;
	boneFormat.combinedTransformationMatrix = m_CombinedTransformationMatrix;

	out.write(reinterpret_cast<const char*>(&boneFormat), sizeof(BONE_FORMAT));

	return S_OK;
}

void FBXBone::SetTransformationMatrix(_float4x4 transformationMatrix)
{
	m_TransformationMatrix = transformationMatrix;
}

void FBXBone::Free()
{
	__super::Free();
}
