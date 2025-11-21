#include "pch.h"
#include "ItemGlow.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"
#include "ParticleSystemComponent.h"

ItemGlow::ItemGlow()
	:Object()
{
}

ItemGlow::ItemGlow(const ItemGlow& prototype)
	:Object(prototype)
{
}

ItemGlow* ItemGlow::Create()
{
	ItemGlow* Instance = new ItemGlow();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ItemGlow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();
	AddComponent<ParticleSystemComponent>();

	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT ItemGlow::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;
	
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_ItemGlow");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("Mask_Pass");
	mtrlInstance->SetFloat2("g_UVOffset", _float2{ 0.f,0.f });

	/*particle*/
	ParticleSystemComponent::PARTICLE_SYSTEM_DESC particleDesc{};
	particleDesc.space = ParticleSpace::Local;
	particleDesc.dirMode = ParticleDirMode::SwayUp;
	particleDesc.spawnPerSec = 2.f;
	particleDesc.isLoop = true;
	particleDesc.spawnAreaMin = _float3{ -0.1f,-0.1f,-0.1f };
	particleDesc.spawnAreaMax = _float3{ 0.1f,-0.1f,0.1f };
	particleDesc.upSpeedMin = 0.05f;
	particleDesc.upSpeedMax = 0.1f;
	particleDesc.swaySpeed = 0.1f;
	particleDesc.sizeMin = _float2{ 3.5f,3.5f };
	particleDesc.sizeMax = _float2{ 3.5f,3.5f };
	particleDesc.lifeMin = 2.f;
	particleDesc.lifeMax = 3.f;

	auto particle = GetComponent<ParticleSystemComponent>();
	particle->Initialize(&particleDesc);

	particle->SetPointParticle(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	particle->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_ItemParticle");


	m_pTransform->SetPosition(_float3{ 0.f,2.f,0.f });
	m_pTransform->SetScale(_float3{ 16.f,25.f,16.f });

	return S_OK;
}

void ItemGlow::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void ItemGlow::Update(_float dt)
{
	__super::Update(dt);

	m_UVOffset.y += dt * 0.2f;

	GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat2("g_UVOffset", m_UVOffset);
}

void ItemGlow::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT ItemGlow::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto sprite = GetComponent<SpriteComponent>();
	auto particle = GetComponent<ParticleSystemComponent>();

	sprite->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(m_eRenderGroup)]);
	particle->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(m_eRenderGroup)]);

	return S_OK;
}

Object* ItemGlow::Clone(InitDESC* arg)
{
	ItemGlow* Instance = new ItemGlow(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void ItemGlow::Free()
{
	__super::Free();
}
