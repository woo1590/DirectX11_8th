#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Model;
class AnimatorComponent;
class TransformComponent;
class MaterialInstance;
class ENGINE_DLL ModelComponent final:
    public Component
{
private:
    ModelComponent(Object* owner);
    ModelComponent(const ModelComponent& prototype);
    virtual ~ModelComponent() = default;

public:
    static ModelComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;
    
    HRESULT ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies);
    HRESULT ExtractShadowProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies);
    HRESULT ConnectAnimator();

    /*Setter*/
    void ShadowEnable() { m_IsDrawShadow = true; }
    void ShadowDisable() { m_IsDrawShadow = false; }
    HRESULT SetModel(_uint levelID, const _string& key);
    void SetOverride(Material* pMaterial);
    void ClearOverride();

    /*Getter*/
    MaterialInstance* GetMaterialInstance()const { return m_pMateriaInstance; }
    _int GetBoneIndex(const _string& boneTag);
    _float4x4 GetOffsetMatrixByIndex(_uint index);
    _float4x4 GetCombinedMatrixByIndex(_uint index);
    _float4x4* GetCombinedMatrixByIndex_PTR(_uint index);

    Component* Clone()override;
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    Model* m_pModel = nullptr;
    _bool m_IsDrawShadow = true;
    std::vector<BONE_PALETTE> m_BonePalettes;

    Material* m_pOverrideMtrl = nullptr;
    MaterialInstance* m_pMateriaInstance = nullptr;
};

NS_END