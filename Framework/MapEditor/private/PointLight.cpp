#include "MapEditorPCH.h"
#include "PointLight.h"	

//component
#include "LightComponent.h"

PointLight::PointLight()
	:Object()
{
}

PointLight::PointLight(const PointLight& prototype)
	:Object(prototype)
{
}

PointLight* PointLight::Create()
{
	PointLight* Instance = new PointLight();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PointLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT PointLight::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*light*/
	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.type = LightType::Point;

	auto light = GetComponent<LightComponent>();
	light->Initialize(&lightDesc);

	engine->RegisterLight(light);

	return S_OK;
}

void PointLight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PointLight::Update(_float dt)
{
	__super::Update(dt);
}

void PointLight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void PointLight::Import(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	_float3 position{};
	LightComponent::LIGHT_DESC desc{};

	position.x = j.at("position").at("x").get<_float>();
	position.y = j.at("position").at("y").get<_float>();
	position.z = j.at("position").at("z").get<_float>();

	desc.type = LightType::Point;
	desc.color.x = j.at("color").at("x").get<_float>();
	desc.color.y = j.at("color").at("y").get<_float>();
	desc.color.z = j.at("color").at("z").get<_float>();
	desc.color.w = j.at("color").at("w").get<_float>();
	desc.range = j.at("range").get<_float>();

	GetComponent<LightComponent>()->Initialize(&desc);
	m_pTransform->SetPosition(position);
}

void PointLight::Export(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	ordered_json node = ordered_json::object();

	LightComponent::LIGHT_DESC lightDesc = GetComponent<LightComponent>()->GetDesc();
	_float3 position = m_pTransform->GetPosition();
	node["position"] = { {"x",position.x},{"y",position.y},{"z",position.z} };
	node["color"] = { {"x",lightDesc.color.x},{"y",lightDesc.color.y},{"z",lightDesc.color.z},{"w",lightDesc.color.w} };
	node["range"] = lightDesc.range;

	j.push_back(std::move(node));
}

Object* PointLight::Clone(InitDESC* arg)
{
	PointLight* Instance = new PointLight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PointLight::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();
}
