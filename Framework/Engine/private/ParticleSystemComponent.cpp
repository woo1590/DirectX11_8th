#include "EnginePCH.h"
#include "ParticleSystemComponent.h"
#include "EngineCore.h"
#include "VIBuffer_InstancingPoint.h"

ParticleSystemComponent::ParticleSystemComponent(Object* owner)
	:Component(owner)
{
}

ParticleSystemComponent::ParticleSystemComponent(const ParticleSystemComponent& prototype)
	:Component(prototype)
{
}

ParticleSystemComponent* ParticleSystemComponent::Create(Object* object)
{
	ParticleSystemComponent* Instance = new ParticleSystemComponent(object);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ParticleSystemComponent::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ParticleSystemComponent::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void ParticleSystemComponent::Update(_float dt)
{
	__super::Update(dt);
}

void ParticleSystemComponent::SetPointParticle(_uint levelID, const _string& key)
{
	m_InstanceDatas.resize(g_iMaxParticle);
	m_Particles.resize(g_iMaxParticle);
	m_DeadParticleIndices.reserve(g_iMaxParticle);

	for (_uint i = 0; i < m_Particles.size(); ++i)
		m_DeadParticleIndices.push_back(i);

	m_pBuffer = VIBuffer_InstancingPoint::Create(levelID, key, g_iMaxParticle, m_InstanceDatas.data());
}

void ParticleSystemComponent::Free()
{
	__super::Free();
}

void ParticleSystemComponent::SpawnParticles(_float dt)
{
	if (m_iBurstCount > 0) //use burst
	{

	}
	else
	{

	}
}

void ParticleSystemComponent::UpdateParticles(_float dt)
{
	for (auto& particle : m_Particles)
	{

	}
}

void ParticleSystemComponent::BuildInstanceData()
{
}

void ParticleSystemComponent::UpdateInstanceData()
{
}
