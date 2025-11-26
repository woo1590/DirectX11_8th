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
    FMOD_RESULT result{};
    result = m_System->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
        return E_FAIL;

    return S_OK;
}


void SoundManager::Update() 
{
    if (m_System)
        m_System->update();

    RemoveDeadChannels();
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

_int SoundManager::Play2DSound(const std::string& key, _float volume)
{
    auto iter = m_SoundMap.find(key);
    if (iter == m_SoundMap.end())
        return -1;

    FMOD::Sound* sound = iter->second;
    FMOD::Channel* channel = nullptr;
    m_System->playSound(sound, nullptr, true, &channel);

    _float finalVolume = std::clamp(m_fMasterVolume * volume, 0.f, 1.f);
    channel->setVolume(finalVolume);

    _uint channelID = m_iNextChannelID++;
    m_Channels.emplace(channelID, channel);
    channel->setPaused(false);

    return channelID;
}

_int SoundManager::Play3DSound(const std::string& key, _float3 position, _float volume)
{
    auto iter = m_SoundMap.find(key);
    if (iter == m_SoundMap.end())
        return -1;

    FMOD::Sound* sound = iter->second;
    FMOD::Channel* channel = nullptr;
    m_System->playSound(sound, nullptr, true, &channel);

    FMOD_VECTOR pos{ position.x,position.y,position.z };
    FMOD_VECTOR vel{ 0.f,0.f,0.f };
    channel->set3DAttributes(&pos, &vel);

    _float finalVolume = std::clamp(m_fMasterVolume * volume, 0.f, 1.f);
    channel->setVolume(finalVolume);

    _uint channelID = m_iNextChannelID++;
    m_Channels.emplace(channelID, channel);
    channel->setPaused(false);

    return channelID;
}

void SoundManager::Stop(_uint id)
{
    auto iter = m_Channels.find(id);
    if (iter == m_Channels.end())
        return;
    
    iter->second->stop();
    m_Channels.erase(iter);
}

void SoundManager::RemoveDeadChannels()
{
    m_DeadChannelKeys.clear();
    for (const auto& pair : m_Channels)
    {
        _bool isPlaying = false;
        pair.second->isPlaying(&isPlaying);
        if (!isPlaying)
            m_DeadChannelKeys.push_back(pair.first);
    }

    for (const auto& key : m_DeadChannelKeys)
        m_Channels.erase(key);
}

void SoundManager::Free()
{
    for (auto& pair : m_SoundMap)
        pair.second->release();

    if (m_System)
    {
        m_System->close();
        m_System->release();
    }
}
