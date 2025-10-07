#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Material;
class Mesh;
class Skeleton;
class TransformComponent;
class ENGINE_DLL Model :
    public Base
{
private:
    Model();
    virtual ~Model() = default;

public:
    static Model* Create(const _string& filePath);
    HRESULT Initialize(const _string& filePath);

    _bool IsSkinned()const { return (m_eType == ModelType::Skinned); }
    RAY_HIT_DATA RayCastLocal(RAY ray, PickingType type = PickingType::BoundingBox);

    /*Getter*/
    Skeleton* GetSkeleton() { return m_pSkeleton; }
    const std::vector<Mesh*>& GetBuffers() { return m_Meshes; }
    const std::vector<Material*> GetMaterials() { return m_Materials; }

    void Free()override;

private:
    HRESULT CreateMeshes(std::ifstream& file, _float4x4 preTransformMatrix);
    HRESULT CreateMaterials(std::ifstream& file, const _string& filePath);
    HRESULT CreateSkeleton(std::ifstream& file, _float4x4 preTransformMatrix);
    void ComputeBoundingBox();

    ModelType m_eType{};

    _uint m_iNumMeshes{};
    _uint m_iNumMaterials{};

    std::vector<Mesh*> m_Meshes;
    std::vector<Material*> m_Materials;
    Skeleton* m_pSkeleton = nullptr;

    /*For BoundingBox*/
    _float3 m_AABBMin{};
    _float3 m_AABBMax{};
    BoundingBox m_BoundingBox{};
};

NS_END