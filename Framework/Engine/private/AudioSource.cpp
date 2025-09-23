#include "EnginePCH.h"
#include "AudioSource.h"
#include "EngineCore.h"

AudioSource::AudioSource(Object* owner)
	:Component(owner)
{
}

AudioSource::AudioSource(const AudioSource& prototype)
	:Component(prototype)
{
}

AudioSource* AudioSource::Create(Object* owner)
{
	AudioSource* Instance = new AudioSource(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AudioSource::Initialize_Prototype()
{
	return S_OK;
}

HRESULT AudioSource::Initialize(InitDESC* arg)
{
	return S_OK;
}

void AudioSource::Update(_float dt)
{
	__super::Update(dt);
}

void AudioSource::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI

void AudioSource::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Audio Source", 
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{

	}

	ImGui::PopID();
}

#endif
