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
    void UpdateTransformationMatrix(_float& trackPosition, _uint& keyframeIndex, std::vector<_float4x4>& matrices);

    void ExtractKeyFrame(std::vector<KEYFRAME>& keyFrames, std::vector<_uint>& mask, _uint index);
    void Free()override;

private:
    _string m_strName{};
    _uint m_iBoneIndex{};
    _uint m_iNumKeyFrames{};
    std::vector<KEYFRAME> m_KeyFrames;

};

NS_END