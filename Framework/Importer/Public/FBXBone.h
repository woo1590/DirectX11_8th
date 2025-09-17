#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXBone :
    public Base
{
private:
    FBXBone();
    virtual ~FBXBone() = default;

public:
    static FBXBone* Create(const aiNode* pNode, _int parentIndex);
    HRESULT Initialize(const aiNode* pNode, _int parentIndex);
    void UpdateCombinedTransformMatrix(std::vector<FBXBone*>& modelBones);

    HRESULT ExportBoneFormat(BONE_FORMAT& boneFormat);
    _string GetBoneTag()const { return m_strBoneTag; }
    _matrix GetCombinedTransformMatrix()const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

    void Free()override;

private:
    _string m_strBoneTag{};
    _float4x4 m_TransformationMatrix{};
    _float4x4 m_CombinedTransformationMatrix{};

    _int m_iParentIndex{};
};

NS_END
