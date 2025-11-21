#pragma once
#include "Editor_EffectNode.h"

NS_BEGIN(EffectEditor)

class Editor_MeshEffect :
    public Editor_EffectNode
{
private:
    Editor_MeshEffect();
    Editor_MeshEffect(const Editor_MeshEffect& prototype);
    virtual ~Editor_MeshEffect() = default;

public:
    static Editor_MeshEffect* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void RenderInspector()override;
    void Start();

    void Import(nlohmann::ordered_json& j);
    void Export(nlohmann::ordered_json& j);

    virtual Object* Clone(InitDESC* arg);
    void Free()override;

private:
    void MeshEffectSetUp();
    void SetCurrModel();

    _string m_strModelTag{};
    _string m_strPassTag{};

    /*sprite anim*/
    _float m_fSpriteElapsedTime{};
    _bool m_IsRepeat = true;
    _uint m_iCurrFrame{};
    _uint m_iMaxFrame{};
    _float m_fSpeed{};
    _float m_fUVRatio{};

    /*uv anim*/
    _float2 m_UVOffset{};
    _float2 m_UVSpeed{};


    _bool m_UseEaseOut = false;
    _float3 m_StartScale{};
    _float3 m_TargetScale{};

};

NS_END