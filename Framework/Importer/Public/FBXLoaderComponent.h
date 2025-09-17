#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class TransformComponent;
NS_END

NS_BEGIN(Importer)

class FBXMesh;
class FBXMaterial;
class FBXBone;
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
    void Update(_float dt)override;

    HRESULT ExtractRenderProxies(TransformComponent* transform, std::vector<RenderProxy>& proxies);
    
    HRESULT ImportModel(const _string& filePath);
    HRESULT ExportModel(const _string& outFilePath);
    HRESULT ExportAnimationClip(const _string& outFilePath) { return S_OK; }//애니메이션은 따로 추출

    _int GetBoneIndexByName(const _string& boneName);

    Component* Clone() { return new FBXLoaderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    HRESULT GenerateMeshes(const aiScene* pScene);
    HRESULT GenerateMaterials(const aiScene* pScene, const _string& modelFilePath);
    HRESULT GenerateBones(const aiNode* pNode, _int parentIndex);
    HRESULT GenerateAnimation(const aiScene* pScene);

    HRESULT WriteMeshFormat(std::ofstream& out);
    HRESULT WriteMaterialFormat(std::ofstream& out);
    HRESULT WriteBoneFormat(std::ofstream& out);

    ModelType m_eType = ModelType::Skinned;

    _uint m_iNumMeshes{};
    _uint m_iNumMaterials{};
    _uint m_iNumBones{};
    std::vector<FBXMesh*> m_Meshes;
    std::vector<FBXMaterial*> m_Materials;
    std::vector<FBXBone*> m_Bones;
    _float4x4 m_PreTransformMatrix{};

    /*--Only Importer--*/
    _bool m_isLoaded = false;
    _string m_strShaderTag{};
    _string m_strFilePath{};
    std::vector<_float4x4> m_BoneMatrices;

    Material* m_pDefaultMtrl = nullptr;
};

NS_END