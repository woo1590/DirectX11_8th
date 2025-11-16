#pragma once
#include "Editor_EffectNode.h"

NS_BEGIN(EffectEditor)

class Editor_LightEffect :
    public Editor_EffectNode
{
private:
    Editor_LightEffect();
    Editor_LightEffect(const Editor_LightEffect& prototype);
    virtual ~Editor_LightEffect() = default;

public:
    static Editor_LightEffect* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void RenderInspector()override;
    void Start() override;

    void Import(nlohmann::ordered_json& j);
    void Export(nlohmann::ordered_json& j) override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    void LightEffectSetUp();

    _float m_fElapsedTime{};
    _float m_fDuration{};
    _float4 m_LightColor{};
    _float m_fStartLightRange{};
    _float m_fTargetLightRange{};
};

NS_END