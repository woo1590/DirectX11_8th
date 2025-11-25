#include "MapEditorPCH.h"
#include "Torch.h"

//compnent
#include "LightComponent.h"

Torch::Torch()
	:Object()
{
}

Torch::Torch(const Torch& prototype)
	:Object(prototype)
{
}

Torch* Torch::Create()
{
	Torch* Instance = new Torch();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Torch::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT Torch::Initialize(InitDESC* arg)
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

void Torch::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Torch::Update(_float dt)
{
	__super::Update(dt);
}

void Torch::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Torch::Import(nlohmann::ordered_json& j)
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

void Torch::Export(nlohmann::ordered_json& j)
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

Object* Torch::Clone(InitDESC* arg)
{
	Torch* Instance = new Torch(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Torch::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();
}
