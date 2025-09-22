#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Channel;
class ENGINE_DLL AnimationClip :
    public Base
{
private:
    AnimationClip();
    virtual ~AnimationClip() = default;

public:
    static AnimationClip* Create(std::ifstream& file);
    HRESULT Initialize(std::ifstream& file);
    void UpdateTransformationMatrix(_float dt, ANIMATIONCLIP_CONTEXT& context, std::vector<_float4x4>& matrices);

    void ExtractKeyFrames(std::vector<KEYFRAME>& keyFrames, std::vector<_uint>& mask, std::vector<_uint>& keyFrameIndices);
    _uint GetNumChannels()const { return m_iNumChannels; }
    void Free()override;

private:
    _string m_strName{};
    _uint m_iNumChannels{};
    _float m_fDuration{};
    _float m_fTickPerSecond{};
    std::vector<Channel*> m_Channels;

};

NS_END