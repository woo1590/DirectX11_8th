#include "pch.h"
#include "StageBoss.h"
#include "EngineCore.h"
#include "Object.h"
#include "EnemySpawner.h"
#include "NavigationComponent.h"

//object
#include "SkillPanel.h"

StageBoss::StageBoss()
	:Level()
{
}

StageBoss* StageBoss::Create()
{
	StageBoss* Instance = new StageBoss();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT StageBoss::Initialize()
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(LoadMapFromFile("../bin/data/map/boss_map.json")))
		return E_FAIL;

	engine->SetNavMesh(ENUM_CLASS(LevelID::Static), "NavMesh_StageBoss");

	if (FAILED(Initialize_LayerLights("Layer_Light")))
		return E_FAIL;

	if (FAILED(Initialize_LayerSkybox("Layer_Skybox")))
		return E_FAIL;

	if (FAILED(Initialize_LayerUI("Layer_UI")))
		return E_FAIL;

	if (FAILED(Initialize_LayerPlayer("Layer_Player")))
		return E_FAIL;

	return S_OK;
}

void StageBoss::Free()
{
}

void StageBoss::Update(_float dt)
{
	if (!m_IsBossSpawned)
	{
		auto engine = EngineCore::GetInstance();

		_float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();

		if (playerPos.z > 297.f)
		{
			engine->AddObject(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss", ENUM_CLASS(LevelID::StageBoss), "Layer_Boss");
			m_IsBossSpawned = true;
		}
	}
}

HRESULT StageBoss::Render()
{
	return E_NOTIMPL;
}

HRESULT StageBoss::LoadMapFromFile(const _string& filePath)
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();

	std::vector<PREFAB> staticObjectPrefabs;
	std::vector<EnemySpawner::ENEMY_SPAWNER_DESC> enemySpawnerDescs;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load");
		return E_FAIL;
	}

	ordered_json map = json::parse(file);
	/*Load object prefab*/
	for (auto& j : map["prefabs"])
	{
		PREFAB prefab{};
		map::PrefabFromJson(j, prefab);
		staticObjectPrefabs.push_back(prefab);
	}
	/*Load Enemy Spawners*/
	for (auto& j : map["enemyspawners"])
	{
		EnemySpawner::ENEMY_SPAWNER_DESC desc{};

		desc.position.x = j.at("Position").at("x").get<_float>();
		desc.position.y = j.at("Position").at("y").get<_float>();
		desc.position.z = j.at("Position").at("z").get<_float>();

		desc.scale.x = j.at("Scale").at("x").get<_float>();
		desc.scale.y = j.at("Scale").at("y").get<_float>();
		desc.scale.z = j.at("Scale").at("z").get<_float>();

		desc.quaternion.x = j.at("Quaternion").at("x").get<_float>();
		desc.quaternion.y = j.at("Quaternion").at("y").get<_float>();
		desc.quaternion.z = j.at("Quaternion").at("z").get<_float>();
		desc.quaternion.w = j.at("Quaternion").at("w").get<_float>();

		desc.availableNavCellIndices = j.value("NavCells", std::vector<_uint>{});

		desc.doorID = j.at("DoorID").get<_int>();

		for (auto& w : j["Waves"])
		{
			EnemySpawner::Wave wave;
			for (auto& entry : w["Entries"])
			{
				wave.push_back({ entry.value("PrototypeTag",""),
								 entry.at("Count").get<_uint>() });
			}

			desc.waves.push_back(wave);
		}

		enemySpawnerDescs.push_back(desc);
	}

	for (auto& prefab : staticObjectPrefabs)
	{
		Object::OBJECT_DESC desc{};
		desc.modelTag = prefab.modelTag;
		desc.position = prefab.position;
		desc.scale = prefab.scale;
		desc.quaternion = prefab.quaternion;
		engine->AddObject(ENUM_CLASS(LevelID::Static), prefab.prototypeTag, ENUM_CLASS(LevelID::StageBoss), prefab.layerTag, &desc);
	}
	for (auto& spawnerDesc : enemySpawnerDescs)
	{
		Object* spawner = nullptr;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemySpawner", ENUM_CLASS(LevelID::StageBoss), "Layer_EnemySpawner", &spawnerDesc, &spawner);

		static_cast<EnemySpawner*>(spawner)->ConnectDoor(ENUM_CLASS(LevelID::StageBoss));
	}

	return S_OK;
}

HRESULT StageBoss::Initialize_LayerLights(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	Object::OBJECT_DESC desc{};
	desc.position = _float3{ 0.f, 234.f, 567.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -40.f, 234.f, 867.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ 140.f, 234.f, 467.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -140.f, 234.f, 667.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ 140.f, 234.f, 667.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -140.f, 234.f, 467.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ 40.f, 234.f, 867.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;
	
	desc.position = _float3{ -40.f, 234.f, 667.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;
	
	desc.position = _float3{ 40.f, 234.f, 467.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;
	
	desc.position = _float3{ -40.f, 234.f, 567.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;
	
	desc.position = _float3{ 40.f, 234.f, 567.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ 100.f, 234.f, 267.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -100.f, 234.f, 267.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -100.f, 234.f, 267.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ 0.f, 234.f, 67.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", ENUM_CLASS(LevelID::StageBoss), layerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT StageBoss::Initialize_LayerSkybox(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Skybox", ENUM_CLASS(LevelID::StageBoss), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT StageBoss::Initialize_LayerPlayer(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	player->GetComponent<NavigationComponent>()->SpawnInCell(0);

	return S_OK;
}

HRESULT StageBoss::Initialize_LayerUI(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	Object* playerPanel = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerPanel", ENUM_CLASS(LevelID::StageBoss), layerTag, nullptr, &playerPanel)))
		return E_FAIL;

	Object* weaponPanel = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponPanel", ENUM_CLASS(LevelID::StageBoss), layerTag, nullptr, &weaponPanel)))
		return E_FAIL;

	Object* skillPanel = nullptr;
	SkillPanel::SKILL_PANEL_DESC skillPanelDesc{};
	skillPanelDesc.numPanel = 0;
	skillPanelDesc.skillID = SkillID::Dash;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SkillPanel", ENUM_CLASS(LevelID::StageBoss), layerTag, &skillPanelDesc, &weaponPanel)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectBackground", ENUM_CLASS(LevelID::StageBoss), layerTag)))
		return E_FAIL;

	return S_OK;
}