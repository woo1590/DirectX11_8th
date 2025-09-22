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
    void UpdateCombiendTransformation(std::vector<FBXBone*>& modelBones);

    HRESULT Export(std::ofstream& out);
    _string GetBoneTag()const { return m_strBoneTag; }
    void SetTransformationMatrix(_float4x4 transformationMatrix);
    _matrix GetCombinedTransformMatrix()const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

    void Free()override;

private:
    _string m_strBoneTag{};
    _int m_iParentIndex{};
    _float4x4 m_TransformationMatrix{};

    /*Only Importer*/
    _float4x4 m_CombinedTransformationMatrix{};

};

NS_END
