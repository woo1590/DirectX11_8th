#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Object;
class SoundManager
    : public Base
{
private:
    SoundManager();
    virtual ~SoundManager(){}

public:
    static SoundManager* Create();
    HRESULT Initialize();
    void Update();

    void Load3DSound(const _string& key, const _string& filePath, _bool loop = false);
    void Load2DSound(const _string& key, const _string& filePath, _bool loop = false);

    _int Play2DSound(const std::string& key, _float volume = 1.f);
    _int Play3DSound(const std::string& key, _float3 position, _float volume = 1.f);
    void Stop(_uint id);

private:
    void RemoveDeadChannels();
    void Free()override;

    FMOD::System* m_System = nullptr;
    Object* m_pListener = nullptr;
    _float m_fMasterVolume = 0.5f;
    _uint m_iNextChannelID{};

    std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;
    std::unordered_map<_uint, FMOD::Channel*> m_Channels;
    std::vector<_uint> m_DeadChannelKeys;
};

NS_END