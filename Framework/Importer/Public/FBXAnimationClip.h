#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXChannel;
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
    void Play(_float dt);

    void WriteAnimationFormat(std::ofstream& out);

    _uint GetNumChannels()const { return m_iNumChannels; }
    std::vector<FBXChannel*>& GetChannels() { return m_Channels; }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector();
#endif

private:
    _string m_strName{};
    _uint m_iNumChannels{};

    _float m_fDuration{};
    _float m_fTickPerSecond{};
    _float m_fCurrTrackPosition{};

    std::vector<FBXChannel*> m_Channels;
};

NS_END