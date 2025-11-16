#include "pch.h"
#include "Editor_LightEffect.h"

#include "LightComponent.h"

Editor_LightEffect::Editor_LightEffect()
	:Editor_EffectNode()
{
}

Editor_LightEffect::Editor_LightEffect(const Editor_LightEffect& prototype)
	:Editor_EffectNode(prototype)
{
}

Editor_LightEffect* Editor_LightEffect::Create()
{
	Editor_LightEffect* Instance = new Editor_LightEffect();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Editor_LightEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT Editor_LightEffect::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto light = GetComponent<LightComponent>();
	EngineCore::GetInstance()->RegisterLight(light);

	return S_OK;
}

void Editor_LightEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Editor_LightEffect::Update(_float dt)
{
	__super::Update(dt);

	if (m_fElapsedTime >= m_fDuration)
		return;

	_float t = m_fElapsedTime / m_fDuration;
	t = std::clamp(t, 0.f, 1.f);

	_float currRange = math::Lerp(m_fStartLightRange, m_fTargetLightRange, t);

	auto light = GetComponent<LightComponent>();
	light->SetLightRange(currRange);
}

void Editor_LightEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Editor_LightEffect::RenderInspector()
{
	ImGui::PushID(this);
	LightEffectSetUp();
	ImGui::PopID();

	__super::RenderInspector();
}

void Editor_LightEffect::Start()
{
	m_fElapsedTime = 0.f;
}

void Editor_LightEffect::Import(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	_float4 color{};

	m_fDuration = j.at("duration").get<_float>();
	m_fStartLightRange = j.at("start_light_range").get<_float>();
	m_fTargetLightRange = j.at("target_light_range").get<_float>();

	color.x = j.at("light_color").at("x").get<_float>();
	color.y = j.at("light_color").at("y").get<_float>();
	color.z = j.at("light_color").at("z").get<_float>();
	color.w = j.at("light_color").at("w").get<_float>();

	m_LightColor = color;

	LightComponent::LIGHT_DESC desc{};
	desc.type = LightType::Point;
	desc.color = color;
	desc.range = m_fStartLightRange;

	auto light = GetComponent<LightComponent>();
	light->Initialize(&desc);
}

void Editor_LightEffect::Export(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	ordered_json node = ordered_json::object();
	node["type"] = "light";

	node["duration"] = m_fDuration;
	node["start_light_range"] = m_fStartLightRange;
	node["target_light_range"] = m_fTargetLightRange;
	node["light_color"] = { {"x",m_LightColor.x},{"y",m_LightColor.y},{"z",m_LightColor.z},{"w",m_LightColor.w} };

	j.push_back(node);
}

Object* Editor_LightEffect::Clone(InitDESC* arg)
{
	Editor_LightEffect* Instance = new Editor_LightEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Editor_LightEffect::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();
}

void Editor_LightEffect::LightEffectSetUp()
{
	_bool isDirty = false;

	ImGui::DragFloat("Duration : ", &m_fDuration);
	ImGui::DragFloat("Start Light Range : ", &m_fStartLightRange);
	ImGui::DragFloat("Target Light Range : ", &m_fTargetLightRange);
	isDirty |= ImGui::DragFloat4("Light Color : ", &m_LightColor.x);

	if (isDirty)
	{
		auto light = GetComponent<LightComponent>();

		LightComponent::LIGHT_DESC desc{};
		desc.color = m_LightColor;

		light->Initialize(&desc);
	}
}
