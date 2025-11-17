#pragma once
#include "Editor_EffectNode.h"

NS_BEGIN(EffectEditor)

class Editor_ParticleEffect :
    public Editor_EffectNode
{
private:
    Editor_ParticleEffect();
    Editor_ParticleEffect(const Editor_ParticleEffect& prototype);
    virtual ~Editor_ParticleEffect() = default;

public:
    static Editor_ParticleEffect* Create();
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
    void ParticleEffectSetUp();
    void AddCurrTexture();
    void ReplaceMaterial();

    _string SeperateString(_string name);

    Material* m_pParticleMaterial = nullptr;
    std::vector<_string> m_ParticleTextureNames;
    _string m_strMaterialTag{};

    /*----emitter params----*/
    ParticleSpace m_eSpace = ParticleSpace::World;
    ParticleDirMode m_eDirMode = ParticleDirMode::None;
    _bool m_UseGravity = false;

    _uint m_iNumBurst{};
    _float m_fSpawnPerSec{};
    _float m_fSpawnAcc{};
    _bool m_IsLoop = false;
    _uint m_iMaxNumSpawnParticle{};

    _float3 m_SpawnAreaMin{};
    _float3 m_SpawnAreaMax{};

    /*dir mode == none*/
    _float3 m_VelocityMin{};
    _float3 m_VelocityMax{};

    /*dir mode ==surface dir*/
    _float3 m_SurfaceDir{};
    _float m_fMinSpeed{};
    _float m_fMaxSpeed{};
    _float m_fSpreadAngle{};

    /*dir mode == sway up*/
    _float m_fUpSpeedMin{};
    _float m_fUpSpeedMax{};
    _float m_fSwaySpeed{};

    _float2 m_SizeMin{};
    _float2 m_SizeMax{};
    _float m_fLifeMin{};
    _float m_fLifeMax{};
};

NS_END