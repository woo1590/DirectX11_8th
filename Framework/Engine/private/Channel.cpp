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
	m_iBoneIndex = channelFormat.boneIndex;
	m_iNumKeyFrames = channelFormat.numKeyFrames;

	m_KeyFrames.resize(m_iNumKeyFrames);
	file.read(reinterpret_cast<char*>(m_KeyFrames.data()), sizeof(KEYFRAME) * m_iNumKeyFrames);

	return S_OK;
}

void Channel::Free()
{
	__super::Free();
}
