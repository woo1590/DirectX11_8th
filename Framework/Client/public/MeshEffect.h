#pragma once
#include "EffectNode.h"

NS_BEGIN(Client)

class MeshEffect :
    public EffectNode
{
private:
    MeshEffect();
    MeshEffect(const MeshEffect& prototype);
    virtual ~MeshEffect() = default;

public:
    static MeshEffect* Create(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
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