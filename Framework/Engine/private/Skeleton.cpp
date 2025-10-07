#include "EnginePCH.h"
#include "Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton * Skeleton::Create(std::ifstream& file, _float4x4 preTransformMatrix)
{
	Skeleton* Instance = new Skeleton();

	if (FAILED(Instance->Initialize(file, preTransformMatrix)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Skeleton::Initialize(std::ifstream& file, _float4x4 preTransformMatrix)
{
	m_PreTransformMatrix = preTransformMatrix;

	SKELETON_FORMAT skeletonFormat{};
	file.read(reinterpret_cast<char*>(&skeletonFormat), sizeof(SKELETON_FORMAT));

	m_iNumBones = skeletonFormat.numBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		BONE_FORMAT	boneFormat{};
		Bone bone{};
		file.read(reinterpret_cast<char*>(&boneFormat), sizeof(BONE_FORMAT));

		bone.m_strBoneTag = boneFormat.boneTag;
		bone.m_iParentIndex = boneFormat.parentIndex;
		bone.m_TransformationMatrix = boneFormat.transformationMatrix;

		m_Bones.push_back(bone);
	}

	m_OffsetMatrices.resize(m_iNumBones);
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_int parentIndex = m_Bones[i].m_iParentIndex;

		if (-1 == parentIndex)
			XMStoreFloat4x4(&m_OffsetMatrices[i], XMLoadFloat4x4(&m_Bones[i].m_TransformationMatrix) * XMLoadFloat4x4(&m_PreTransformMatrix));
		else
			XMStoreFloat4x4(&m_OffsetMatrices[i], XMLoadFloat4x4(&m_Bones[i].m_TransformationMatrix) * XMLoadFloat4x4(&m_OffsetMatrices[parentIndex]));
	}

	return S_OK;
}

_float4x4 Skeleton::GetOffsetMatrixByIndex(_uint index)
{
	if (index >= m_iNumBones)
	{
		_float4x4 matrix{};
		XMStoreFloat4x4(&matrix, XMMatrixIdentity());

		return matrix;
	}

	return m_OffsetMatrices[index];
}

_int Skeleton::GetBoneIndexByName(const _string& boneTag)
{
	_uint index = 0;
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](Bone bone)
		{
			if (boneTag == bone.m_strBoneTag)
				return true;

			index++;
			return false;
		});

	if (iter == m_Bones.end())
		return -1;
	else
		return index;
}

void Skeleton::Free()
{
	__super::Free();

	m_Bones.clear();
}
