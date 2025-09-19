#include "ImporterPCH.h"
#include "FBXChannel.h"
#include "FBXLoaderComponent.h"
#include "FBXBone.h"

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
	_uint a{};

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME keyFrame{};

		if (i < pChannel->mNumScalingKeys)
		{
			memcpy_s(&scale, sizeof(_float3), &pChannel->mScalingKeys[i].mValue, sizeof(_float3));
			keyFrame.trackPosition = pChannel->mScalingKeys[i].mTime;

			if (pChannel->mScalingKeys[i].mInterpolation != aiAnimInterpolation_Linear)
				int a = 0;
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

			if (pChannel->mPositionKeys[i].mInterpolation != aiAnimInterpolation_Linear)
				int a = 0;
		}

		keyFrame.scale = scale;
		keyFrame.rotation = rotation;
		keyFrame.position = position;

		m_KeyFrames.push_back(keyFrame);
	}

	return S_OK;
}

void FBXChannel::UpdateTransformationMatrix(_float currTrackPostion, std::vector<FBXBone*>& bones)
{
	if (!currTrackPostion)
		m_iCurrKeyFrameIndex = 0;

	_vector scale, rotation, position;
	KEYFRAME lastKeyFrame = m_KeyFrames.back();

	if (currTrackPostion >= lastKeyFrame.trackPosition)
	{
		scale = XMLoadFloat3(&lastKeyFrame.scale);
		rotation = XMLoadFloat4(&lastKeyFrame.rotation);
		position = XMVectorSetW(XMLoadFloat3(&lastKeyFrame.position), 1.f);
	}
	else
	{
		if (currTrackPostion >= m_KeyFrames[m_iCurrKeyFrameIndex + 1].trackPosition)
			m_iCurrKeyFrameIndex++;

		KEYFRAME currKeyFrame = m_KeyFrames[m_iCurrKeyFrameIndex];
		KEYFRAME nextKeyFrame = m_KeyFrames[m_iCurrKeyFrameIndex + 1];

		_vector scaleA		= XMLoadFloat3(&currKeyFrame.scale);
		_vector rotationA	= XMLoadFloat4(&currKeyFrame.rotation);
		_vector positionA	= XMVectorSetW(XMLoadFloat3(&currKeyFrame.position), 1.f);

		_vector scaleB		= XMLoadFloat3(&nextKeyFrame.scale);
		_vector rotaionB	= XMLoadFloat4(&nextKeyFrame.rotation);
		_vector positionB	= XMVectorSetW(XMLoadFloat3(&nextKeyFrame.position), 1.f);

		_float t = (currTrackPostion - currKeyFrame.trackPosition) / (nextKeyFrame.trackPosition - currKeyFrame.trackPosition);
		t = std::clamp(t, 0.f, 1.f);

		scale = XMVectorLerp(scaleA, scaleB, t);
		rotation = XMQuaternionSlerp(rotationA, rotaionB, t);
		position = XMVectorLerp(positionA, positionB, t);
	}

	_float4x4 transformationMatrix{};
	_vector rotationOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	XMStoreFloat4x4(&transformationMatrix, XMMatrixAffineTransformation(scale, rotationOrigin, rotation, position));
	bones[m_iBoneIndex]->SetTransformationMatrix(transformationMatrix);
}

void FBXChannel::WriteChannelFormat(std::ofstream& out)
{
	CHANNEL_FORMAT channelFormat{};
	strncpy_s(channelFormat.channelTag, sizeof(channelFormat.channelTag), m_strName.c_str(), sizeof(channelFormat.channelTag) - 1);
	channelFormat.boneIndex = m_iBoneIndex;
	channelFormat.numKeyFrames = m_iNumKeyFrames;

	out.write(reinterpret_cast<const char*>(&channelFormat), sizeof(CHANNEL_FORMAT));
	out.write(reinterpret_cast<const char*>(m_KeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);
}

void FBXChannel::Free()
{
	__super::Free();
}
