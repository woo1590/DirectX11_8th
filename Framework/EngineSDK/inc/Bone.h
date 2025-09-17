#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Bone final:
    public Base
{
private:
    Bone();
    virtual ~Bone() = default;

public:
    static Bone* Create(BONE_FORMAT boneFormat);
    HRESULT Initialize(BONE_FORMAT boneFormat);
    void UpdateCombinedTransformMatrix(std::vector<Bone*>& bones, _fmatrix preTransformMatrix);

    _matrix GetCombinedTransformationMatrix()const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

    void Free()override;

private:
    _string m_strBoneTag{};
    _int m_iParentIndex{};
    _float4x4 m_TransformationMatrix{};
    _float4x4 m_CombinedTransformationMatrix{};
};

NS_END