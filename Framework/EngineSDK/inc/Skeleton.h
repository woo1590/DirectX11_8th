#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Skeleton :
    public Base
{
private:
    Skeleton();
    virtual ~Skeleton() = default;

public:
    static Skeleton * Create(std::ifstream& file, _float4x4 preTransformMatrix);
    HRESULT Initialize(std::ifstream& file, _float4x4 preTransformMatrix);

    /*Getter*/
    const std::vector<Bone>& GetBones() { return m_Bones; }
    _float4x4 GetOffsetMatrixByIndex(_uint index);
    _float4x4 GetPreTransformMatrix()const { return m_PreTransformMatrix; }
    _int GetBoneIndexByName(const _string& boneTag);

    void Free()override;

private:
    _uint m_iNumBones{};
    _float4x4 m_PreTransformMatrix{};
    std::vector<Bone> m_Bones;
    std::vector<_float4x4> m_OffsetMatrices;
};

NS_END