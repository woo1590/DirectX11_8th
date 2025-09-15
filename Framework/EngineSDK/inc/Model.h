#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Material;
class Mesh;
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

    HRESULT ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies, Material* overrideMaterial = nullptr);
    void Free()override;

private:
    HRESULT CreateMeshes(std::ifstream& file);
    HRESULT CreateMaterials(std::ifstream& file, const _string& filePath);

    _uint m_iNumMeshes{};
    _uint m_iNumMaterials{};
    std::vector<Mesh*> m_Meshes;
    std::vector<Material*> m_Materials;
};

NS_END