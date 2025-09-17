#pragma once
#include "VIBuffer.h"
#include "RenderProxy.h"

NS_BEGIN(Importer)

class FBXBone;
class FBXLoaderComponent;
class FBXMesh final:
    public VIBuffer
{
private:
    FBXMesh();
    virtual ~FBXMesh() = default;

public:
    static FBXMesh* Create(aiMesh* pMesh, ModelType eType, FBXLoaderComponent* pLoader);
    HRESULT Initialize(aiMesh* pMesh, ModelType eType, FBXLoaderComponent* pLoader);
    _uint GetMaterialIndex()const { return m_iMaterialIndex; }
    void ExtractBoneMatrices(RenderProxy& proxy, std::vector<FBXBone*>& bones);

    HRESULT ExportMeshFormat(MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertices, std::vector<_uint>& indices);

    void Free()override;
private:
    HRESULT CreateStaticMesh(aiMesh* pMesh);
    HRESULT CreateSkinnedMesh(aiMesh* pMesh, FBXLoaderComponent* pLoader);

    _string m_strName{};
    _uint m_iMaterialIndex{};
    _uint m_iNumBones{};                        //-> 이 메쉬에 영향을 주는 뼈 갯수
    std::vector<_uint> m_BoneIndices;           //-> 이 메쉬에 영향을 주는 실제 모델 bone벡터의 인덱스
    std::vector<_float4x4> m_OffsetMatrices;    //-> 각 bone별 offsetmatrix
    std::vector<_float4x4> m_BoneMatrices;

    std::vector<VTX_FORMAT> m_VertexFormats;
    std::vector<_uint> m_Indices;

};

NS_END