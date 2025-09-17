#include "EnginePCH.h"
#include "Bone.h"

Bone::Bone()
{
}

Bone* Bone::Create(BONE_FORMAT boneFormat)
{
	Bone* Instance = new Bone();

	if (FAILED(Instance->Initialize(boneFormat)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Bone::Initialize(BONE_FORMAT boneFormat)
{
	m_strBoneTag = boneFormat.boneTag;
	m_iParentIndex = boneFormat.parentIndex;
	m_TransformationMatrix = boneFormat.transformationMatrix;
	m_CombinedTransformationMatrix = boneFormat.combinedTransformationMatrix;

	return S_OK;
}

void Bone::UpdateCombinedTransformMatrix(std::vector<Bone*>& bones, _fmatrix preTransformMatrix)
{
	if (m_iParentIndex == -1)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * preTransformMatrix);
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * bones[m_iParentIndex]->GetCombinedTransformationMatrix());
	}
}

void Bone::Free()
{
	__super::Free();
}
