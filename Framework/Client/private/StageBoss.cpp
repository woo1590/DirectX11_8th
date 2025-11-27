#include "pch.h"
#include "StageBoss.h"
#include "EngineCore.h"
#include "Object.h"
#include "EnemySpawner.h"
#include "NavigationComponent.h"
#include "GameManager.h"

//object
#include "SkillPanel.h"
#include "PointLight.h"
#include "Torch.h"

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
	if (FAILED(LoadLightFromFile("../bin/data/map/boss_map_light.json")))
		return E_FAIL;

	engine->SetNavMesh(ENUM_CLASS(LevelID::Static), "NavMesh_StageBoss");

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
	EngineCore::GetInstance()->StopSound(m_iBossBGM);

	__super::Free();
}

void StageBoss::Update(_float dt)
{
	if (!m_IsBossSpawned)
	{
		auto engine = EngineCore::GetInstance();

		m_fDropWater += dt;
		if (m_fDropWater >= 5.f)
		{
			engine->Play2DSound("SFX_DropWater", 0.6f);
			m_fDropWater = 0.f;
		}

		_float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();

		if (playerPos.z > 297.f)
		{
			engine->AddObject(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss", ENUM_CLASS(LevelID::StageBoss), "Layer_Boss");

			engine->StopSound(m_iBossEnterBGM);
			GameManager::GetInstance()->StartBossBattle();
			m_IsBossSpawned = true;
		}
	}
	
	auto engine = EngineCore::GetInstance();
	if (engine->IsKeyPressed('N'))
	{
		engine->IsNavDebugEnable() ? engine->NavDebugDisable() : engine->NavDebugEnable();
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

HRESULT StageBoss::LoadLightFromFile(const _string& filePath)
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load");
		return E_FAIL;
	}

	ordered_json map = json::parse(file);

	/*point light*/
	for (const auto& light : map["point_lights"])
	{
		_float3 position{};
		PointLight::POINT_LIGHT_DESC desc{};

		position.x = light.at("position").at("x").get<_float>();
		position.y = light.at("position").at("y").get<_float>();
		position.z = light.at("position").at("z").get<_float>();

		desc.position = position;

		desc.color.x = light.at("color").at("x").get<_float>();
		desc.color.y = light.at("color").at("y").get<_float>();
		desc.color.z = light.at("color").at("z").get<_float>();
		desc.color.w = light.at("color").at("w").get<_float>();

		desc.range = light.at("range").get<_float>();

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::StageBoss), "Layer_PointLight", &desc);
	}

	/*torch*/
	for (const auto& light : map["torches"])
	{
		_float3 position{};
		Torch::TORCH_DESC desc{};

		position.x = light.at("position").at("x").get<_float>();
		position.y = light.at("position").at("y").get<_float>();
		position.z = light.at("position").at("z").get<_float>();

		desc.position = position;
		desc.color.x = light.at("color").at("x").get<_float>();
		desc.color.y = light.at("color").at("y").get<_float>();
		desc.color.z = light.at("color").at("z").get<_float>();
		desc.color.w = light.at("color").at("w").get<_float>();

		desc.range = light.at("range").get<_float>();

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Torch", ENUM_CLASS(LevelID::StageBoss), "Layer_Torch", &desc);
	}

	return S_OK;
}

HRESULT StageBoss::Initialize_LayerSkybox(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Skybox", ENUM_CLASS(LevelID::StageBoss), layerTag)))
		return E_FAIL;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cloud", ENUM_CLASS(LevelID::StageBoss), layerTag)))
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

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitCrossHair", ENUM_CLASS(LevelID::Stage1), layerTag)))
		return E_FAIL;

	return S_OK;
}