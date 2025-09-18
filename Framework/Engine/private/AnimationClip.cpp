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

void AnimationClip::Free()
{
	__super::Free();

	for (auto& channel : m_Channels)
		Safe_Release(channel);
	m_Channels.clear();
}
