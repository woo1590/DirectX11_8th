#pragma once
#include "VIBuffer.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class ENGINE_DLL Mesh :
    public VIBuffer
{
private:
    Mesh();
    virtual ~Mesh() = default;

public:
    static Mesh * Create(ModelType eType, std::ifstream& file, _fmatrix preTransformMatrix);
    HRESULT Initialize(ModelType eType, std::ifstream& file, _fmatrix preTransformMatrix);

    void ComputeBonePalette(const std::vector<_float4x4>& combinedMatices, std::vector<_float4x4>& bonePalette);
    _uint GetMaterialIndex()const { return m_iMaterialIndex; }

    void Free()override;

private:
    HRESULT CreateStaticMesh(std::ifstream& file, _fmatrix preTransformMatrix);
    HRESULT CreateSkinnedMesh(std::ifstream& file);

    _string m_strName{};

    /*----Only SkinnedMesh----*/
    _uint m_iMaterialIndex{};
    _uint m_iNumBones{};
    std::vector<_uint> m_BoneIndices;
    std::vector<_float4x4> m_OffsetMatrices;
    std::vector<_float4x4> m_BoneMatrices;
};

NS_END