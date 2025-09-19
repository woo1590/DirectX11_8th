#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXChannel;
class FBXBone;
class FBXLoaderComponent;
class FBXAnimationClip :
    public Base
{
private:
    FBXAnimationClip();
    virtual ~FBXAnimationClip() = default;

public:
    static FBXAnimationClip* Create(aiAnimation* pAnimation,FBXLoaderComponent* pLoader);
    HRESULT Initialize(aiAnimation* pAnimation, FBXLoaderComponent* pLoader);

    void WriteAnimationFormat(std::ofstream& out);

    void Play(_float dt, std::vector<FBXBone*>& bones);
    void Stop() { m_isPlaying = false; m_fCurrTrackPosition = 0.f; }
    _bool IsPlaying()const { return m_isPlaying; }

    _uint GetNumChannels()const { return m_iNumChannels; }
    std::vector<FBXChannel*>& GetChannels() { return m_Channels; }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector(_uint id);
    void RenameButton();
    void PlayButton();
    void ProgressBar();
#endif

private:
    _string m_strName{};
    _uint m_iNumChannels{};

    _float m_fDuration{};
    _float m_fTickPerSecond{};
    _float m_fCurrTrackPosition{};
    _bool m_isPlaying = false;

    std::vector<FBXChannel*> m_Channels;
};

NS_END