#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class TransformComponent;
NS_END

NS_BEGIN(Importer)

class FBXMesh;
class FBXMaterial;
class FBXLoaderComponent :
    public Component
{
private:
    FBXLoaderComponent(Object* owner);
    FBXLoaderComponent(const FBXLoaderComponent& prototype);
    virtual ~FBXLoaderComponent() = default;

public:
    static FBXLoaderComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    HRESULT ExtractRenderProxies(TransformComponent* transform, std::vector<RenderProxy>& proxies);
    
    HRESULT ImportModel(const _string& filePath);
    HRESULT ExportModel(const _string& outFilePath);

    Component* Clone() { return new FBXLoaderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    HRESULT GenerateMeshes(const aiScene* pScene);
    HRESULT GenerateMaterials(const aiScene* pScene, const _string& modelFilePath);

    HRESULT WriteMeshFormat(std::ofstream& out);
    HRESULT WriteMaterialFormat(std::ofstream& out);

    HRESULT ConvertToDDS(const _string& textureFilePath);


    _uint m_iNumMeshes{};
    std::vector<FBXMesh*> m_Meshes;
    _uint m_iNumMaterials{};
    std::vector<FBXMaterial*> m_Materials;

    _bool m_isLoaded = false;
    _string m_strShaderTag{};
    _string m_strFilePath{};

    Material* m_pDefaultMtrl = nullptr;
};

NS_END