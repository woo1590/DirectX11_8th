#pragma once
#include "Component.h"

NS_BEGIN(Engine)

typedef struct tagParticle
{
    _bool isAlive = false;
    _float maxLifeTime{};
    _float lifeTime{};
    _float3 velocity{};
    _float3 position{};
    _float2 size{};
}PARTICLE;

class VIBuffer_InstancingPoint;
class Model;
class Material;
class MaterialInstance;
class ParticleSystemComponent :
    public Component
{
private:
    ParticleSystemComponent(Object* owner);
    ParticleSystemComponent(const ParticleSystemComponent& prototype);
    virtual ~ParticleSystemComponent() = default;

public:
    static ParticleSystemComponent* Create(Object* object);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    void SetPointParticle(_uint levelID, const _string& key);
    void SetMeshParticle(_uint levelID, const _string& key);
    Component* Clone()override { return new ParticleSystemComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    void SpawnParticles(_float dt);
    void UpdateParticles(_float dt);
    void BuildInstanceData();
    void UpdateInstanceData();

    VIBuffer_InstancingPoint* m_pBuffer = nullptr;  //for point particle
    Model* m_pModel = nullptr;                      //for mesh particle
    Material* m_pMaterial = nullptr;
    MaterialInstance* m_pMaterialInstance = nullptr;

    _uint m_iNumAliveParticle{};
    std::vector<_uint> m_DeadParticleIndices;
    std::vector<PARTICLE> m_Particles;
    std::vector<VTX_INSTANCE_POINT> m_InstanceDatas;

    /*----emitter params----*/
    _uint m_iBurstCount{};
    _float m_fSpawnPerSec{};
    _float m_fSpawnElapsedTime{};
    _bool m_IsBurst = false;
    _bool m_IsLoop = false;

    _float3 m_SpawnAreaMin{};
    _float3 m_SpawnAreMax{};
    _float3 m_VelocityMin{};
    _float3 m_VelocityMax{};
    _float2 m_fSizeMin{};
    _float2 m_fSizeMax{};
    _float m_fLifeMin{};
    _float m_fLifeMax{};
};

NS_END