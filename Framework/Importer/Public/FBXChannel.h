#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXLoaderComponent;
class FBXBone;
class FBXChannel :
    public Base
{
private:
    FBXChannel();
    virtual ~FBXChannel() = default;

public:
    static FBXChannel * Create(aiNodeAnim* pChannel, FBXLoaderComponent* pLoader);
    HRESULT Initialize(aiNodeAnim* pChannel, FBXLoaderComponent* pLoader);
    void UpdateTransformationMatrix(_float currTrackPostion, std::vector<FBXBone*>& bones);

    void WriteChannelFormat(std::ofstream& out);

    std::vector<KEYFRAME>& GetKeyFrames() { return m_KeyFrames; }
    void ResetKeyFrame() { m_iCurrKeyFrameIndex = 0; }
    void Free()override;

private:
    _string m_strName{};
    _uint m_iBoneIndex{};
    _uint m_iNumKeyFrames{};
    _uint m_iCurrKeyFrameIndex{};
    std::vector<KEYFRAME> m_KeyFrames;
};

NS_END