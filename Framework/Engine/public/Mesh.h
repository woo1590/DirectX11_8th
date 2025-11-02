#pragma once
#include "VIBuffer.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class ENGINE_DLL Mesh :
    public VIBuffer
{
private:
    typedef struct tagClusterNode
    {
        BoundingBox boundingBox{};
        std::vector<_uint> indices;
    }CLUSTER_NODE;
private:
    Mesh();
    virtual ~Mesh() = default;

public:
    static Mesh* Create(ModelType eType, std::ifstream& file, _fmatrix preTransformMatrix);
    HRESULT Initialize(ModelType eType, std::ifstream& file, _fmatrix preTransformMatrix);

    void ComputeBonePalette(const std::vector<_float4x4>& combinedMatices, std::vector<_float4x4>& bonePalette);
    _uint GetMaterialIndex()const { return m_iMaterialIndex; }
    RAY_HIT_DATA RayCast(RAY localRay, PickingType type);
    BOUNDING_BOX_DATA GetMeshBoundingBoxData()const;

    void Free()override;

private:
    HRESULT CreateStaticMesh(std::ifstream& file, _fmatrix preTransformMatrix);
    HRESULT CreateSkinnedMesh(std::ifstream& file);

    void ComputeBoundingBox(ModelType eType);
    _uint BuildBVHNode(std::vector<_uint>& triIndices, _uint startIndex, _uint endIndex);
    void RayCastBVHNode(_uint nodeIndex, RAY localRay, RAY_HIT_DATA& out);

    _string m_strName{};

    /*---For Bounding Box---*/
    _float3 m_AABBMin{};
    _float3 m_AABBMax{};
    BoundingBox m_BoundingBox{};

    _bool m_IsStaticMesh = false;
    _uint m_iLeafCount = 8;
    std::vector<BVH_NODE> m_BVHNodes;
    std::vector<TRIANGLE_DESC> m_Triangles;
    std::vector<_uint> m_BVHTriIndices;

    std::vector<_float3> m_VertexPositions;
    std::vector<_uint> m_Indices;

    /*----Only SkinnedMesh----*/
    _uint m_iMaterialIndex{};
    _uint m_iNumBones{};
    std::vector<_uint> m_BoneIndices;
    std::vector<_float4x4> m_OffsetMatrices;
    std::vector<_float4x4> m_BoneMatrices;
};

NS_END