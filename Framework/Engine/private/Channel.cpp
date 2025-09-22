#include "EnginePCH.h"
#include "Channel.h"

Channel::Channel()
{
}

Channel* Channel::Create(std::ifstream& file)
{
	Channel* Instance = new Channel();

	if (FAILED(Instance->Initialize(file)))
		Safe_Release(Instance);
	
	return Instance;
}

HRESULT Channel::Initialize(std::ifstream& file)
{
	CHANNEL_FORMAT channelFormat{};

	file.read(reinterpret_cast<char*>(&channelFormat), sizeof(CHANNEL_FORMAT));
	m_strName = channelFormat.channelTag;
	m_iBoneIndex = channelFormat.boneIndex;
	m_iNumKeyFrames = channelFormat.numKeyFrames;

	m_KeyFrames.resize(m_iNumKeyFrames);
	file.read(reinterpret_cast<char*>(m_KeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);

	return S_OK;
}

void Channel::UpdateTransformationMatrix(_float& trackPosition, _uint& keyframeIndex, std::vector<_float4x4>& matrices)
{
	_vector scale, rotation, position;
	KEYFRAME lastKeyFrame = m_KeyFrames.back();

	if (!trackPosition)
		keyframeIndex = 0;

	if (trackPosition >= lastKeyFrame.trackPosition)
	{
		scale = XMLoadFloat3(&lastKeyFrame.scale);
		rotation = XMLoadFloat4(&lastKeyFrame.rotation);
		position = XMVectorSetW(XMLoadFloat3(&lastKeyFrame.position), 1.f);
	}
	else
	{
		if (trackPosition >= m_KeyFrames[keyframeIndex + 1].trackPosition)
			keyframeIndex++;

		KEYFRAME currKeyFrame = m_KeyFrames[keyframeIndex];
		KEYFRAME nextKeyFrame = m_KeyFrames[keyframeIndex + 1];

		_vector scaleA = XMLoadFloat3(&currKeyFrame.scale);
		_vector rotationA = XMLoadFloat4(&currKeyFrame.rotation);
		_vector positionA = XMLoadFloat3(&currKeyFrame.position);

		_vector scaleB = XMLoadFloat3(&nextKeyFrame.scale);
		_vector rotationB = XMLoadFloat4(&nextKeyFrame.rotation);
		_vector positionB = XMLoadFloat3(&nextKeyFrame.position);

		_float t = (trackPosition - currKeyFrame.trackPosition) / (nextKeyFrame.trackPosition - currKeyFrame.trackPosition);
		t = std::clamp(t, 0.f, 1.f);

		scale = XMVectorLerp(scaleA, scaleB, t);
		rotation = XMQuaternionSlerp(rotationA, rotationB, t);
		position = XMVectorSetW(XMVectorLerp(positionA, positionB, t), 1.f);
	}

	_vector rotationOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_matrix transformation = XMMatrixAffineTransformation(scale, rotationOrigin, rotation, position);

	XMStoreFloat4x4(&matrices[m_iBoneIndex], transformation);
}

void Channel::ExtractKeyFrame(std::vector<KEYFRAME>& keyFrames, std::vector<_uint>& mask, _uint index)
{
	keyFrames[m_iBoneIndex] = m_KeyFrames[index];
	mask[m_iBoneIndex] = 1;
}

void Channel::Free()
{
	__super::Free();
}
