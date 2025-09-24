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
class FBXAnimationClip;
class FBXSkeleton;
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

    Component* Clone() { return new FBXLoaderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
    void ImportInspector(_string openedFile);
    void ExportInspector(_string savedFileName);
#endif

private:
    HRESULT CreateMeshes(const aiScene* pScene);
    HRESULT CreateMaterials(const aiScene* pScene, const _string& modelFilePath);
    HRESULT CreateSkeleton(const aiScene* pScene, _int parentIndex);
    HRESULT CreateAnimations(const aiScene* pScene);

    HRESULT ExportMeshes(std::ofstream& out);
    HRESULT ExportMaterials(std::ofstream& out);
    HRESULT ExportSkeleton(std::ofstream& out);
    HRESULT ExportAnimations(const _string& outFilePath);

    void FindPlayingAnimation();
    void PlayAnimation(_float dt);
    void Clear();
    void DeleteMesh(_uint index);

    ModelType m_eType = ModelType::Skinned;

    _uint m_iNumMeshes{};
    _uint m_iNumMaterials{};
    std::vector<FBXMesh*> m_Meshes;
    std::vector<FBXMaterial*> m_Materials;
    FBXSkeleton* m_pSkeleton = nullptr;
    _float4x4 m_PreTransformMatrix{};
    
    /*For Animation*/
    _uint m_iNumAnimations{};
    std::vector<FBXAnimationClip*> m_AnimationClips;
    _int m_iCurrAnimationIndex{};

    /*--Only Importer--*/
    _bool m_isLoaded = false;
    _string m_strShaderTag{};
    _string m_strFilePath{};
    std::vector<_float4x4> m_BoneMatrices;

    Material* m_pDefaultMtrl = nullptr;
};

NS_END