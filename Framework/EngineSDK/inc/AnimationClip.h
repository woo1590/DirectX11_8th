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
    static AnimationClip * Create(std::ifstream& file);
    HRESULT Initialize(std::ifstream& file);

    void Free()override;

private:
    _string m_strName{};
    _uint m_iNumChannels{};
    _float m_fDuration{};
    _float m_fTickPerSecond{};
    std::vector<Channel*> m_Channels;

};

NS_END