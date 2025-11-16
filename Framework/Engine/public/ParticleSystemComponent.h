#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

typedef struct tagParticle
{
    _bool isAlive = false;
    _float maxLifeTime{};
    _float lifeTime{};
    _float3 velocity{};
    _float3 position{};
    _float2 size{};

    /*sway mode*/
    _float3 startPosition{};
    _float upSpeed{};
    _float startPhase{};

}PARTICLE;

class TransformComponent;
class VIBuffer_InstancingPoint;
class Model;
class Material;
class MaterialInstance;
class ENGINE_DLL ParticleSystemComponent :
    public Component
{
public:
    typedef struct tagParticleSystemDesc : public InitDESC
    {
        ParticleSpace space = ParticleSpace::World;
        ParticleDirMode dirMode = ParticleDirMode::None;
        _bool useGravity = false;

        _uint numBurst{};
        _float spawnPerSec{};
        _float spawnAcc{};
        _bool isLoop = false;

        _float3 spawnAreaMin{};
        _float3 spawnAreaMax{};

        /*dir mode == none*/
        _float3 velocityMin{};
        _float3 velocityMax{};

        //dir mode == surface
        _float3 surfaceDir{};
        _float minSpeed{};
        _float maxSpeed{};
        _float spreadAngle{};

        //dir mode == sway up
        _float upSpeedMin{};
        _float upSpeedMax{};
        _float swaySpeed{};

        _float2 sizeMin{};
        _float2 sizeMax{};
        _float lifeMin{};
        _float lifeMax{};

    }PARTICLE_SYSTEM_DESC;

private:
    ParticleSystemComponent(Object* owner);
    ParticleSystemComponent(const ParticleSystemComponent& prototype);
    virtual ~ParticleSystemComponent() = default;

public:
    static ParticleSystemComponent* Create(Object* object);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;
    HRESULT ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies);

    void SetPointParticle(_uint levelID, const _string& key);
    void SetMeshParticle(_uint levelID, const _string& key);
    void SetMaterial(_uint levelID, const _string& key);

    void SetSurfaceDir(_float3 dir) { m_SurfaceDir = dir; }
    void SetSpawnArea(_float3 areaMin, _float3 areaMax) { m_SpawnAreaMin = areaMin; m_SpawnAreaMax = areaMax; }

    Component* Clone()override { return new ParticleSystemComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    void SpawnParticles(_float dt);
    void UpdateParticles(_float dt);
    void BuildInstanceData();
    void UploadInstanceData();
    void SetUpParticle(PARTICLE& particle) const;

    VIBuffer_InstancingPoint* m_pBuffer = nullptr;  //for point particle
    Model* m_pModel = nullptr;                      //for mesh particle
    Material* m_pMaterial = nullptr;
    MaterialInstance* m_pMaterialInstance = nullptr;

    _uint m_iNumAliveParticle{};
    std::vector<_uint> m_DeadParticleIndices;
    std::vector<PARTICLE> m_Particles;
    std::vector<VTX_INSTANCE_POINT> m_InstanceDatas;

    /*----emitter params----*/
    ParticleSpace m_eSpace = ParticleSpace::World;
    ParticleDirMode m_eDirMode = ParticleDirMode::None;
    _bool m_UseGravity = false;

    _uint m_iNumBurst{};
    _float m_fSpawnPerSec{};
    _float m_fSpawnAcc{};
    _bool m_IsLoop = false;

    _float3 m_SpawnAreaMin{};
    _float3 m_SpawnAreaMax{};

    /*dir mode == none*/
    _float3 m_VelocityMin{};
    _float3 m_VelocityMax{};

    /*dir mode == surface dir*/
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