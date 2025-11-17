#include "pch.h"
#include "LightEffect.h"
#include "EngineCore.h"

#include "LightComponent.h"

LightEffect::LightEffect()
	:EffectNode()
{
}

LightEffect::LightEffect(const LightEffect& prototype)
	:EffectNode(prototype),
	m_fDuration(prototype.m_fDuration),
	m_LightColor(prototype.m_LightColor),
	m_fStartLightRange(prototype.m_fStartLightRange),
	m_fTargetLightRange(prototype.m_fTargetLightRange),
	m_Position(prototype.m_Position)
{
}

LightEffect* LightEffect::Create(nlohmann::ordered_json prefab, const _string& filePath)
{
	LightEffect* Instance = new LightEffect();
	
	if (FAILED(Instance->Initialize_Prototype(prefab, filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightEffect::Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	_float3 position{}, scale{};
	_float4 quaternion{};

	m_fDuration = prefab.at("duration").get<_float>();

	position.x = prefab.at("position").at("x").get<_float>();
	position.y = prefab.at("position").at("y").get<_float>();
	position.z = prefab.at("position").at("z").get<_float>();

	scale.x = prefab.at("scale").at("x").get<_float>();
	scale.y = prefab.at("scale").at("y").get<_float>();
	scale.z = prefab.at("scale").at("z").get<_float>();

	quaternion.x = prefab.at("quaternion").at("x").get<_float>();
	quaternion.y = prefab.at("quaternion").at("y").get<_float>();
	quaternion.z = prefab.at("quaternion").at("z").get<_float>();
	quaternion.w = prefab.at("quaternion").at("w").get<_float>();

	m_LightColor.x = prefab.at("light_color").at("x").get<_float>();
	m_LightColor.y = prefab.at("light_color").at("y").get<_float>();
	m_LightColor.z = prefab.at("light_color").at("z").get<_float>();
	m_LightColor.w = prefab.at("light_color").at("w").get<_float>();

	m_fStartLightRange = prefab.at("start_light_range").get<_float>();
	m_fTargetLightRange = prefab.at("target_light_range").get<_float>();

	m_Position = position;

	return S_OK;
}

HRESULT LightEffect::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	LightComponent::LIGHT_DESC desc{};
	desc.type = LightType::Point;
	desc.range = m_fStartLightRange;
	desc.color = m_LightColor;
	
	auto light = GetComponent<LightComponent>();
	light->Initialize(&desc);

	EngineCore::GetInstance()->RegisterLight(light);

	m_pTransform->SetPosition(m_Position);

	return S_OK;
}

HRESULT LightEffect::LateInitialize()
{
	return S_OK;
}

void LightEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LightEffect::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;

	if (m_fElapsedTime >= m_fDuration)
	{
		GetComponent<LightComponent>()->SetDeactive();
		return;
	}

	_float t = m_fElapsedTime / m_fDuration;
	t = std::clamp(t, 0.f, 1.f);

	_float currRange = math::Lerp(m_fStartLightRange, m_fTargetLightRange, t);

	auto light = GetComponent<LightComponent>();
	light->SetLightRange(currRange);
}

void LightEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* LightEffect::Clone(InitDESC* arg)
{
	LightEffect* Instance = new LightEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LightEffect::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());
	
	__super::Free();
}
