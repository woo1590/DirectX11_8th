#include "pch.h"
#include "GamePlayLevel.h"
#include "EngineCore.h"
#include "Object.h"

//object
#include "EnemySpawner.h"

//component
#include "NavigationComponent.h"

GamePlayLevel::GamePlayLevel()
{
}

GamePlayLevel* GamePlayLevel::Create()
{
	GamePlayLevel* Instance = new GamePlayLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT GamePlayLevel::Initialize()
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(LoadMapFromFile("../bin/data/map/stage1_map.json")))
		return E_FAIL;

	engine->SetNavMesh(ENUM_CLASS(LevelID::GamePlay), "NavMesh_Test");

	if (FAILED(Initialize_LayerLights("Layer_Lights")))
		return E_FAIL;

	if (FAILED(Initialize_LayerSkybox("Layer_Skybox")))
		return E_FAIL;

	if (FAILED(Initialize_LayerPlayer("Layer_Player")))
		return E_FAIL;

	if (FAILED(Initialize_LayerEnemy("Layer_Enemy")))
		return E_FAIL;

	if (FAILED(Intialize_LayerCamera("Layer_Camera")))
		return E_FAIL;

	return S_OK;
}

void GamePlayLevel::Free()
{
	__super::Free();
}

void GamePlayLevel::Update(_float dt)
{
	auto engine = EngineCore::GetInstance();
	
	if (engine->IsKeyPressed('4'))
		engine->SetMainCamera("ThirdCamera");
	if (engine->IsKeyPressed('5'))
		engine->SetMainCamera("PlayerCamera");

	if (engine->IsKeyPressed('I'))
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Soldier", ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy");
	if (engine->IsKeyPressed('O'))
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead", ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy");

}

HRESULT GamePlayLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"GamePlay Level");

	return S_OK;
}

HRESULT GamePlayLevel::LoadMapFromFile(const _string& filePath)
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
								 entry.at("Count").get<_uint>()});
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
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), prefab.prototypeTag, ENUM_CLASS(LevelID::GamePlay), prefab.layerTag, &desc);
	}
	for (auto& spawner : enemySpawnerDescs)
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_EnemySpawner", ENUM_CLASS(LevelID::GamePlay), "Layer_EnemySpawner", &spawner);

	return S_OK;
}

HRESULT GamePlayLevel::Initialize_LayerLights(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Sun",
		ENUM_CLASS(LevelID::GamePlay), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT GamePlayLevel::Initialize_LayerSkybox(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Skybox",
								 ENUM_CLASS(LevelID::GamePlay), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT GamePlayLevel::Intialize_LayerCamera(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if(FAILED(engine->AddObject(ENUM_CLASS(LevelID::GamePlay),"Prototype_Object_ThirdCam",
								ENUM_CLASS(LevelID::GamePlay),layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT GamePlayLevel::Initialize_LayerPlayer(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();
	
	Object* player = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Player",
								 ENUM_CLASS(LevelID::GamePlay), layerTag, nullptr, &player)))
		return E_FAIL;

	player->GetComponent<NavigationComponent>()->SpawnInCell(8);

	return S_OK;
}

HRESULT GamePlayLevel::Initialize_LayerEnemy(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	return S_OK;
}
