#include "ImporterPCH.h"
#include "FBXAnimationClip.h"
#include "FBXLoaderComponent.h"
#include "FBXChannel.h"

FBXAnimationClip::FBXAnimationClip()
{
}

FBXAnimationClip* FBXAnimationClip::Create(aiAnimation* pAnimation, FBXSkeleton* pSkeleton)
{
	FBXAnimationClip* Instance = new FBXAnimationClip();

	if (FAILED(Instance->Initialize(pAnimation, pSkeleton)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXAnimationClip::Initialize(aiAnimation* pAnimation, FBXSkeleton* pSkeleton)
{
	m_strName = pAnimation->mName.data;
	m_fDuration = pAnimation->mDuration;
	m_fTickPerSecond = pAnimation->mTicksPerSecond;
	if (!m_fTickPerSecond)
		m_fTickPerSecond = 1.f;
	m_iNumChannels = pAnimation->mNumChannels;
	
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		auto channel = FBXChannel::Create(pAnimation->mChannels[i], pSkeleton);
		if (!channel)
		{
			MSG_BOX("Failed to create : FBXChannel");
			return E_FAIL;
		}

		m_Channels.push_back(channel);
	}

	return S_OK;
}

void FBXAnimationClip::Play(_float dt, std::vector<FBXBone*>& bones)
{
	m_fCurrTrackPosition += m_fTickPerSecond * dt;

	if (m_fCurrTrackPosition >= m_fDuration)
	{
		m_fCurrTrackPosition = 0.f;
	}

	for (const auto& channel : m_Channels)
		channel->UpdateTransformationMatrix(m_fCurrTrackPosition, bones);
}

void FBXAnimationClip::WriteAnimationFormat(std::ofstream& out)
{
	ANIMATIONCLIP_FORMAT animationClipFormat{};
	strncpy_s(animationClipFormat.animationTag, sizeof(animationClipFormat.animationTag), m_strName.c_str(), sizeof(animationClipFormat.animationTag) - 1);
	animationClipFormat.numChannels = m_iNumChannels;
	animationClipFormat.tickPerSecond = m_fTickPerSecond;
	animationClipFormat.duration = m_fDuration;
	
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

void FBXAnimationClip::RenderInspector(_uint id)
{
	ImGui::PushID(this);
	_string name = std::to_string(id) + " : " + m_strName;
	if (ImGui::CollapsingHeader(name.c_str(),
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		RenameButton();
		PlayButton();
		ProgressBar();
	}
	ImGui::PopID();
}

void FBXAnimationClip::RenameButton()
{
	if (ImGui::Button("Rename"))
		ImGui::OpenPopup("Rename##clip");

	if (ImGui::BeginPopupModal("Rename##clip", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		static char rename[128];

		if (ImGui::IsWindowAppearing())
		{
			strncpy_s(rename, 128, m_strName.c_str(), 128);
			ImGui::SetKeyboardFocusHere();
		}

		ImGui::TextUnformatted("Input new clip name..");
		if (ImGui::InputText("##name", rename, 128,
			ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_strName = rename;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

}

void FBXAnimationClip::PlayButton()
{
	if (ImGui::Button("Play"))
	{
		m_isPlaying = true;
		m_fCurrTrackPosition = 0.f;
		for (const auto& channel : m_Channels)
			channel->ResetKeyFrame();
	}
}

void FBXAnimationClip::ProgressBar()
{
	if (m_isPlaying)
	{
		_float ratio = m_fCurrTrackPosition / m_fDuration;
		ImGui::ProgressBar(ratio);
	}
}
