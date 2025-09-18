#include "ImporterPCH.h"
#include "FBXAnimationClip.h"
#include "FBXLoaderComponent.h"
#include "FBXChannel.h"

FBXAnimationClip::FBXAnimationClip()
{
}

FBXAnimationClip* FBXAnimationClip::Create(aiAnimation* pAnimation, FBXLoaderComponent* pLoader)
{
	FBXAnimationClip* Instance = new FBXAnimationClip();

	if (FAILED(Instance->Initialize(pAnimation, pLoader)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXAnimationClip::Initialize(aiAnimation* pAnimation, FBXLoaderComponent* pLoader)
{
	m_strName = pAnimation->mName.data;
	m_fDuration = pAnimation->mDuration;
	m_fTickPerSecond = pAnimation->mTicksPerSecond;
	m_iNumChannels = pAnimation->mNumChannels;
	
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		auto channel = FBXChannel::Create(pAnimation->mChannels[i], pLoader);
		if (!channel)
		{
			MSG_BOX("Failed to create : FBXChannel");
			return E_FAIL;
		}

		m_Channels.push_back(channel);
	}

	return S_OK;
}

void FBXAnimationClip::Play(_float dt)
{
	m_fCurrTrackPosition += m_fTickPerSecond * dt;

	if (m_fCurrTrackPosition >= m_fDuration)
	{

	}
}

void FBXAnimationClip::WriteAnimationFormat(std::ofstream& out)
{
	ANIMATIONCLIP_FORMAT animationClipFormat{};
	strncpy_s(animationClipFormat.animationTag, sizeof(animationClipFormat.animationTag), m_strName.c_str(), sizeof(animationClipFormat.animationTag) - 1);
	animationClipFormat.numChannels = m_iNumChannels;
	
	out.write(reinterpret_cast<const char*>(&animationClipFormat), sizeof(ANIMATIONCLIP_FORMAT));

	for (_uint i = 0; i < m_iNumChannels; ++i)
		m_Channels[i]->WriteChannelFormat(out);
}

void FBXAnimationClip::Free()
{
	__super::Free();

	for (auto& channel : m_Channels)
		Safe_Release(channel);
	m_Channels.clear();
}

void FBXAnimationClip::RenderInspector()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(m_strName.c_str(),
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{

	}
	ImGui::PopID();
}
