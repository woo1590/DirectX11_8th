#pragma once
#include "EffectNode.h"

NS_BEGIN(Client)

class SpriteEffect :
    public EffectNode
{
public:
    typedef struct tagSpriteEffectDesc : public EffectNode::EFFECT_NODE_DESC
    {
    }SPRITE_EFFECT_DESC;
private:
    SpriteEffect();
    SpriteEffect(const SpriteEffect& prototype);
    virtual ~SpriteEffect() = default;

public:
    static SpriteEffect* Create(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_IsActive = true;

    _string m_strMaterialTag{};

    _float m_fSpeed{};
    _int m_iMaxFrameIndex{};
    _bool m_IsRepeat{};
    _bool m_IsAnimated{};

    /*---animated params---*/
    _bool m_UseBillBoard = true;
    _bool m_UseEaseOut = false;
    _float3 m_StartPosition{};
    _float3 m_TargetPositon{};
    _float3 m_StartScale{};
    _float3 m_TargetScale{};
    _float3 m_StartRotation{};
    _float3 m_TargetRotation{};

    _float m_fStartAlpha = 1.f;
    _float m_fTargetAlpha = 1.f;

    _float2 m_UVSpeed{};
    _float2 m_UVOffset{};
};

NS_END