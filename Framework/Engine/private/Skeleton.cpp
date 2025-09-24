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

	return S_OK;
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
