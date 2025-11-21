#include "pch.h"
#include "AttackRange.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"
#include "LightComponent.h"

AttackRange::AttackRange()
	:Object()
{
}

AttackRange::AttackRange(const AttackRange& prototype)
	:Object(prototype)
{
}

AttackRange* AttackRange::Create()
{
	AttackRange* Instance = new AttackRange();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AttackRange::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();
	AddComponent<LightComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT AttackRange::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	ATTACK_RANGE_DESC* desc = static_cast<ATTACK_RANGE_DESC*>(arg);

	m_pInnerCircleTransform = TransformComponent::Create(this);
	m_pInnerCircleSprite = SpriteComponent::Create(this);

	/*light*/	
	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.type = LightType::Point;
	lightDesc.range = desc->attackRange - 10.f;
	lightDesc.color = _float4{ 1.f,0.f,0.f,1.f };

	auto light = GetComponent<LightComponent>();
	light->Initialize(&lightDesc);
	engine->RegisterLight(light);

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_AttackRange");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("AttackRange_Pass");
	mtrlInstance->SetFloat4("g_Color", _float4(1.f, 0.f, 0.f, 1.f));

	m_pInnerCircleSprite->Initialize(&spriteDesc);
	m_pInnerCircleSprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	m_pInnerCircleSprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_AttackRange");

	auto mtrlInstanceInner = m_pInnerCircleSprite->GetMaterialInstance();
	mtrlInstanceInner->SetPass("AttackRange_Pass");
	mtrlInstanceInner->SetFloat4("g_Color", _float4(1.f, 0.f, 0.f, 1.f));

	m_fTargetRange = desc->attackRange;
	m_fLifeDuration = desc->lifeDuration;

	m_pTransform->Rotate(_float3{ math::ToRadian(90.f),0.f,0.f });
	m_pTransform->SetScale(_float3{ m_fTargetRange,m_fTargetRange,1.f });

	m_pInnerCircleTransform->Rotate(_float3{ math::ToRadian(90.f),0.f,0.f });
	m_pInnerCircleTransform->SetPosition(desc->position);

	return S_OK;
}

void AttackRange::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void AttackRange::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		m_pInnerCircleTransform->SetScale(_float3{ m_fStartRange,m_fStartRange,1.f });
		m_fElapsedTime = 0.f;
	}
	else
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		_float currRange = math::Lerp(m_fStartRange, m_fTargetRange, t);
		m_pInnerCircleTransform->SetScale(_float3{ currRange,currRange,1.f });
	}

	m_fLifeElapsedTime += dt;
	if (m_fLifeElapsedTime >= m_fLifeDuration)
		SetDead();
}

void AttackRange::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT AttackRange::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	m_pInnerCircleSprite->ExtractRenderProxy(m_pInnerCircleTransform, proxies[ENUM_CLASS(m_eRenderGroup)]);

	return S_OK;
}

Object* AttackRange::Clone(InitDESC* arg)
{
	AttackRange* Instance = new AttackRange(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void AttackRange::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();

	Safe_Release(m_pInnerCircleTransform);
	Safe_Release(m_pInnerCircleSprite);
}
