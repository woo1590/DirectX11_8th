#include "ImporterPCH.h"
#include "FBXChannel.h"
#include "FBXLoaderComponent.h"

FBXChannel::FBXChannel()
{
}

FBXChannel * FBXChannel::Create(aiNodeAnim* pChannel, FBXLoaderComponent* pLoader)
{
	FBXChannel* Instance = new FBXChannel();

	if (FAILED(Instance->Initialize(pChannel, pLoader)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXChannel::Initialize(aiNodeAnim* pChannel, FBXLoaderComponent* pLoader)
{
	m_strName = pChannel->mNodeName.data;
	m_iBoneIndex = pLoader->GetBoneIndexByName(m_strName);
	
	m_iNumKeyFrames = (std::max)(pChannel->mNumScalingKeys, pChannel->mNumRotationKeys);
	m_iNumKeyFrames = (std::max)(pChannel->mNumPositionKeys, m_iNumKeyFrames);

	_float3 scale{};
	_float4 rotation{};
	_float3 position{};

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME keyFrame{};

		if (i < pChannel->mNumScalingKeys)
		{
			memcpy_s(&scale, sizeof(_float3), &pChannel->mScalingKeys[i].mValue, sizeof(_float3));
			keyFrame.trackPosition = pChannel->mScalingKeys[i].mTime;
		}

		if (i < pChannel->mNumRotationKeys)
		{
			rotation.x = pChannel->mRotationKeys[i].mValue.x;
			rotation.y = pChannel->mRotationKeys[i].mValue.y;
			rotation.z = pChannel->mRotationKeys[i].mValue.z;
			rotation.w = pChannel->mRotationKeys[i].mValue.w;
			keyFrame.trackPosition = pChannel->mRotationKeys[i].mTime;
		}

		if (i < pChannel->mNumPositionKeys)
		{
			memcpy_s(&position, sizeof(_float3), &pChannel->mPositionKeys[i].mValue, sizeof(_float3));
			keyFrame.trackPosition = pChannel->mPositionKeys[i].mTime;
		}

		keyFrame.scale = scale;
		keyFrame.rotation = rotation;
		keyFrame.position = position;

		if (keyFrame.trackPosition > 0.f)
			int a = 1;

		m_KeyFrames.push_back(keyFrame);
	}

	return S_OK;
}

void FBXChannel::WriteChannelFormat(std::ofstream& out)
{
	CHANNEL_FORMAT channelFormat{};
	channelFormat.boneIndex = m_iBoneIndex;
	channelFormat.numKeyFrames = m_iNumKeyFrames;

	out.write(reinterpret_cast<const char*>(&channelFormat), sizeof(CHANNEL_FORMAT));
	out.write(reinterpret_cast<const char*>(m_KeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);
}

void FBXChannel::Free()
{
	__super::Free();
}
