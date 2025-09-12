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

    HRESULT ExtractMeshFormat(MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertices, std::vector<_uint>& indices);

    void Free()override;
private:
    std::vector<VTXMESH> m_Vertices;
    std::vector<_uint> m_Indices;
};

NS_END