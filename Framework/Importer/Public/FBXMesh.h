#pragma once
#include "VIBuffer.h"

NS_BEGIN(Importer)

class FBXMesh final:
    public VIBuffer
{
private:
    FBXMesh();
    virtual ~FBXMesh() = default;

public:
    static FBXMesh* Create(aiMesh* pMesh);
    HRESULT Initialize(aiMesh* pMesh);
    _uint GetMaterialIndex()const { return m_iMaterialIndex; }

    HRESULT ExportMeshFormat(MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertices, std::vector<_uint>& indices);

    void Free()override;
private:
    _uint m_iMaterialIndex{};

    std::vector<VTXMESH> m_Vertices;
    std::vector<_uint> m_Indices;
};

NS_END