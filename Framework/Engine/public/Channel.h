#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Channel :
    public Base
{
private:
    Channel();
    virtual ~Channel() = default;

public:
    static Channel* Create(std::ifstream& file);
    HRESULT Initialize(std::ifstream& file);

    void Free()override;

private:
    _uint m_iBoneIndex{};
    _uint m_iNumKeyFrames{};
    std::vector<KEYFRAME> m_KeyFrames;

};

NS_END