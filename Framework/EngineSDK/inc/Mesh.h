#pragma once
#include "VIBuffer.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Bone;
class ENGINE_DLL Mesh :
    public VIBuffer
{
private:
    Mesh();
    virtual ~Mesh() = default;

public:
    static Mesh * Create(ModelType eType, const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats,std::vector<_uint>& indexFormats, _fmatrix preTransformMatrix);
    HRESULT Initialize(ModelType eType, const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats, _fmatrix preTransformMatrix);

    void ExtractBoneMatrices(RenderProxy& proxy, std::vector<Bone*>& bones);
    _uint GetMaterialIndex()const { return m_iMaterialIndex; }
    void Free()override;

private:
    HRESULT CreateStaticMesh(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats, _fmatrix preTransformMatrix);
    HRESULT CreateSkinnedMesh(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats);

    _string m_strName{};

    /*----Only SkinnedMesh----*/
    _uint m_iMaterialIndex{};
    _uint m_iNumBones{};
    std::vector<_uint> m_BoneIndices;
    std::vector<_float4x4> m_OffsetMatrices;
    std::vector<_float4x4> m_BoneMatrices;
};

NS_END