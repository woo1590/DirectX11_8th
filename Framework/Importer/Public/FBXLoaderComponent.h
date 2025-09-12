#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class TransformComponent;
class Material;
NS_END

NS_BEGIN(Importer)

class FBXMesh;
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
    
    HRESULT LoadModelFromFBX(const _string& filePath);
    HRESULT ExportModel(const _string& outFilePath);

    Component* Clone() { return new FBXLoaderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    HRESULT GenerateMeshes(const aiScene* pScene);
    HRESULT GenerateMaterias(const aiScene* pScene) { return S_OK; }

    HRESULT WriteMeshFormat(std::ofstream& out);

    _bool m_isLoaded = false;
    _uint m_iNumMeshes{};
    std::vector<FBXMesh*> m_Meshes;




    std::vector<Material*> m_Materials;
    Material* m_pDefaultMtrl = nullptr;
};

NS_END