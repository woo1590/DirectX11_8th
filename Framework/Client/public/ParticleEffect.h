#pragma once
#include "EffectNode.h"

NS_BEGIN(Client)

class ParticleEffect :
    public EffectNode
{
private:
    ParticleEffect();
    ParticleEffect(const ParticleEffect& prototype);
    virtual ~ParticleEffect() = default;

public:
    static ParticleEffect* Create(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath);
    HRESULT Initialize(InitDESC* arg)override;
    HRESULT LateInitialize()override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;
private:
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