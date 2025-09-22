#include "EnginePCH.h"
#include "AudioListener.h"

AudioListener::AudioListener(Object* owner)
	:Component(owner)
{
}

AudioListener::AudioListener(const AudioListener& prototype)
	:Component(prototype)
{
}

AudioListener* AudioListener::Create(Object* owner)
{
	AudioListener* Instance = new AudioListener(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AudioListener::Initialize_Prototype()
{
	return S_OK;
}

HRESULT AudioListener::Initialize(InitDESC* arg)
{
	return S_OK;
}

void AudioListener::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI

void AudioListener::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Audio Listener",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{

	}

	ImGui::PopID();
}

#endif
