#include "EnginePCH.h"
#include "AnimationClip.h"
#include "Channel.h"

AnimationClip::AnimationClip()
{
}

AnimationClip* AnimationClip::Create(std::ifstream& file)
{
	AnimationClip* Instance = new AnimationClip();

	if (FAILED(Instance->Initialize(file)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AnimationClip::Initialize(std::ifstream& file)
{
	ANIMATIONCLIP_FORMAT animationClipFormat{};

	file.read(reinterpret_cast<char*>(&animationClipFormat), sizeof(ANIMATIONCLIP_FORMAT));
	m_strName = animationClipFormat.animationTag;
	m_iNumChannels = animationClipFormat.numChannels;
	m_fDuration = animationClipFormat.duration;
	m_fTickPerSecond = animationClipFormat.tickPerSecond;

	m_Channels.resize(m_iNumChannels);
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		auto channel = Channel::Create(file);
		if (!channel)
		{
			MSG_BOX("Failed to create : Channel");
			return E_FAIL;
		}

		m_Channels[i] = channel;
	}

	return S_OK;
}

void AnimationClip::UpdateTransformationMatrix(_float dt, ANIMATIONCLIP_CONTEXT& context, std::vector<_float4x4>& matrices)
{
	if (context.isFinished)
		return;

	context.trackPosition += dt * m_fTickPerSecond * context.playSpeedScale;

	if (context.trackPosition >= m_fDuration)
	{
		if (!context.isLoop)
		{
			context.isFinished = true;
			return;
		}

		context.trackPosition = 0.f;
	}

	for (_uint i = 0; i < m_Channels.size(); ++i)
		m_Channels[i]->UpdateTransformationMatrix(context.trackPosition, context.keyFrameIndices[i], matrices);
}

void AnimationClip::ExtractKeyFrames(std::vector<KEYFRAME>& keyFrames, std::vector<_uint>& mask, std::vector<_uint>& keyFrameIndices)
{
	for (_uint i = 0; i < m_iNumChannels; ++i)
		m_Channels[i]->ExtractKeyFrame(keyFrames, mask, keyFrameIndices[i]);
}

void AnimationClip::Free()
{
	__super::Free();

	for (auto& channel : m_Channels)
		Safe_Release(channel);
	m_Channels.clear();
}
