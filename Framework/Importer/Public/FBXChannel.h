#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXLoaderComponent;
class FBXChannel :
    public Base
{
private:
    FBXChannel();
    virtual ~FBXChannel() = default;

public:
    static FBXChannel * Create(aiNodeAnim* pChannel, FBXLoaderComponent* pLoader);
    HRESULT Initialize(aiNodeAnim* pChannel, FBXLoaderComponent* pLoader);

    void WriteChannelFormat(std::ofstream& out);
    std::vector<KEYFRAME>& GetKeyFrames() { return m_KeyFrames; }

    void Free()override;

private:
    _string m_strName{};
    _uint m_iBoneIndex{};
    _uint m_iNumKeyFrames{};
    std::vector<KEYFRAME> m_KeyFrames;
};

NS_END