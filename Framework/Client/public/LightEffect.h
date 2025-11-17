#pragma once
#include "EffectNode.h"

NS_BEGIN(Client)

class LightEffect :
    public EffectNode
{
private:
    LightEffect();
    LightEffect(const LightEffect& prototype);
    virtual ~LightEffect() = default;

public:
    static LightEffect* Create(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize(InitDESC* arg)override;
    HRESULT LateInitialize()override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
    _float m_fDuration{};
    _float4 m_LightColor{};
    _float m_fStartLightRange{};
    _float m_fTargetLightRange{};

    _float3 m_Position{};
};

NS_END