#pragma once
#include "Editor_EffectNode.h"

NS_BEGIN(Engine)
class Material;
NS_END

NS_BEGIN(EffectEditor)

class Editor_SpriteEffect :
    public Editor_EffectNode
{
private:
    Editor_SpriteEffect();
    Editor_SpriteEffect(const Editor_SpriteEffect& prototype);
    virtual ~Editor_SpriteEffect() = default;

public:
    static Editor_SpriteEffect* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    void RenderInspector()override;
    void Start() override;

    void Import(nlohmann::ordered_json& j);
    void Export(nlohmann::ordered_json& j) override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    void SpriteEffectSetUp();
    void AddCurrTexture();
    void ReplaceMaterial();

    _string SeperateString(_string name);

    Material* m_pSpriteMaterial = nullptr;
    std::vector<_string> m_SpriteTextureNames;
    _string m_strMaterialTag{};

    _bool m_IsActive = true;

    _bool m_IsAnimated{};
    _float m_fSpeed{};
    _int m_iMaxFrameIndex{};
    _bool m_IsRepeat{};

    /*---animated params---*/
    _bool m_UseBillboard = true;
    _bool m_UseEaseOut = false;
    _float3 m_StartPosition{};
    _float3 m_TargetPositon{};
    _float3 m_StartScale{};
    _float3 m_TargetScale{};
    _float3 m_StartRotaion{};
    _float3 m_TargetRotation{};

    _float m_fStartAlpha = 1.f;
    _float m_fTargetAlpha = 1.f;

    _float2 m_UVSpeed{};
    _float2 m_UVOffset{};
};

NS_END