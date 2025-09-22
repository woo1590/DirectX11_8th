#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXLoaderComponent;
class FBXBone;
class FBXSkeleton;
class FBXChannel :
    public Base
{
private:
    FBXChannel();
    virtual ~FBXChannel() = default;

public:
    static FBXChannel * Create(aiNodeAnim* pChannel, FBXSkeleton* pSkeleton);
    HRESULT Initialize(aiNodeAnim* pChannel, FBXSkeleton* pSkeleton);
    void UpdateTransformationMatrix(_float currTrackPostion, std::vector<FBXBone*>& bones);

    void WriteChannelFormat(std::ofstream& out);

    std::vector<KEYFRAME>& GetKeyFrames() { return m_KeyFrames; }
    void ResetKeyFrame() { m_iCurrKeyFrameIndex = 0; }
    void Free()override;

private:
    _string m_strName{};
    _uint m_iBoneIndex{};
    _uint m_iNumKeyFrames{};

    std::vector<KEYFRAME> m_KeyFrames;

    /*Only Importer*/
    _uint m_iCurrKeyFrameIndex{};
};

NS_END