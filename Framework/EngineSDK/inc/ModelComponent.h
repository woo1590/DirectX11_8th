#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Model;
class AnimatorComponent;
class TransformComponent;
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
    HRESULT ConnectAnimator();

    /*Setter*/
    HRESULT SetModel(_uint levelID, const _string& key);
    void SetOverride(Material* pMaterial);
    void ClearOverride();

    /*Getter*/
    _int GetBoneIndex(const _string& boneTag);
    _float4x4 GetBoneMatrixByIndex(_uint index);

    Component* Clone()override;
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    Model* m_pModel = nullptr;
    std::vector<BONE_PALETTE> m_BonePalettes;

    Material* m_pOverrideMtrl = nullptr;
};

NS_END