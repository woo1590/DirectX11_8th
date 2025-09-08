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
	for (auto& ch : m_NonLoopChannels)	//non loop채널만 확인, loop 채널은 명시적으로 해제
	{
		if (IsPlaying(ch))
		{

		}
		else
		{
			ch->stop();					//재생 끝나면 채널 반환
			ch = nullptr;
		}
	}
}

void AudioSource::Play(const _string& soundTag)
{
	auto engine = EngineCore::GetInstance();

}

void AudioSource::Free()
{
	__super::Free();
}

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

_bool AudioSource::IsPlaying(FMOD::Channel* ch)
{
	_bool playing = false;

	if (!ch)
		return false;
	
	return ch->isPlaying(&playing) == FMOD_OK && playing;
}
