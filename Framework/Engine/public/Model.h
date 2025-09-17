#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Material;
class Mesh;
class Bone;
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
    void Update(_float dt);

    HRESULT ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies, Material* overrideMaterial = nullptr);
    void Free()override;

private:
    HRESULT CreateMeshes(std::ifstream& file);
    HRESULT CreateMaterials(std::ifstream& file, const _string& filePath);
    HRESULT CreateBones(std::ifstream& file);

    ModelType m_eType{};

    _uint m_iNumMeshes{};
    _uint m_iNumMaterials{};
    _uint m_iNumBones{};

    std::vector<Mesh*> m_Meshes;
    std::vector<Material*> m_Materials;
    std::vector<Bone*> m_Bones;

    _float4x4 m_PreTransformMatrix{};
};

NS_END