#include "EnginePCH.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager* SoundManager::Create()
{
    SoundManager* Instance = new SoundManager;

    if (FAILED(Instance->Initialize()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT SoundManager::Initialize()
{
    FMOD::System_Create(&m_System);
    m_System->init(32, FMOD_INIT_NORMAL, nullptr);

    m_System->createChannelGroup("Master", &m_Master);
    m_System->createChannelGroup("SFX", &m_SFXGroup);
    m_System->createChannelGroup("BGM", &m_BGMGroup);

    m_Master->addGroup(m_SFXGroup);
    m_Master->addGroup(m_BGMGroup);

    return S_OK;
}


void SoundManager::Update() {
    if (m_System)
        m_System->update();
}

void SoundManager::Load3DSound(const _string& key, const _string& filePath, _bool loop)
{
    if (m_SoundMap.count(key))
        return;

    FMOD::Sound* sound = nullptr;

    FMOD_MODE mode = loop ? FMOD_3D | FMOD_LOOP_NORMAL : FMOD_3D | FMOD_DEFAULT;
    m_System->createSound(filePath.c_str(), mode, 0, &sound);

    m_SoundMap[key] = sound;
}

void SoundManager::Load2DSound(const _string& key, const _string& filePath, _bool loop)
{
    if (m_SoundMap.count(key))
        return;

    FMOD::Sound* sound = nullptr;
    FMOD_MODE mode = loop ? FMOD_2D | FMOD_LOOP_NORMAL : FMOD_2D | FMOD_DEFAULT;
    m_System->createSound(filePath.c_str(), mode, 0, &sound);

    m_SoundMap[key] = sound;
}

FMOD::Channel* SoundManager::PlaySFX(const std::string& key)
{
    if (m_SoundMap.count(key) == 0) 
        return nullptr;

    FMOD::Channel* ch = nullptr;
    m_System->playSound(m_SoundMap[key], m_SFXGroup, true, &ch);    //sfx는 이후 위치 지정을 위해 pause

    return ch;
}

void SoundManager::PlayBGM(const std::string& key)
{
    if (m_SoundMap.count(key) == 0) return;

    FMOD::Channel* ch = nullptr;
    m_System->playSound(m_SoundMap[key], m_BGMGroup, false, &ch);//bgm은 pause없이 바로 재생

    m_ChannelMap["BGM"] = ch;
}

void SoundManager::Stop(const std::string& key)
{
    if (m_ChannelMap.count(key))
        m_ChannelMap[key]->stop();
}

void SoundManager::SetSFXVolume(float volume)
{
    if (m_SFXGroup)
        m_SFXGroup->setVolume(volume);
}

void SoundManager::SetBGMVolume(float volume)
{
    if (m_BGMGroup)
        m_BGMGroup->setVolume(volume);
}

void SoundManager::MuteSFX(bool mute)
{
    if (m_SFXGroup)
        m_SFXGroup->setMute(mute);
}

void SoundManager::MuteBGM(bool mute)
{
    if (m_BGMGroup)
        m_BGMGroup->setMute(mute);
}

void SoundManager::FadeOut(const std::string& key, float duration)
{
    // 구현 생략: 매 프레임마다 볼륨 줄이기 or 타이머 기반 처리
}

void SoundManager::Free()
{
    for (auto& pair : m_SoundMap)
        pair.second->release();

    if (m_System)
        m_System->release();
}
