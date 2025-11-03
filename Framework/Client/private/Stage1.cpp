#include "pch.h"
#include "Stage1.h"
#include "EngineCore.h"
#include "LoadingLevel.h"

//object
#include "Door.h"
#include "EnemySpawner.h"
#include "DropWeapon.h"

//component
#include "NavigationComponent.h"

Stage1::Stage1()
	:Level()
{
}

Stage1* Stage1::Create()
{
	Stage1* Instance = new Stage1();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Stage1::Initialize()
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(LoadMapFromFile("../bin/data/map/stage1_map.json")))
		return E_FAIL;

	engine->SetNavMesh(ENUM_CLASS(LevelID::Static), "NavMesh_Stage1");

	if (FAILED(Initialize_LayerLights("Layer_Light")))
		return E_FAIL;

	if (FAILED(Initialize_LayerPlayer("Layer_Player")))
		return E_FAIL;

	if (FAILED(Initialize_LayerUI("Layer_UI")))
		return E_FAIL;

	return S_OK;
}

void Stage1::Free()
{
	__super::Free();
}

void Stage1::Update(_float dt)
{
	auto engine = EngineCore::GetInstance();

	if (engine->IsKeyPressed('N'))
		engine->ChangeLevel(ENUM_CLASS(LevelID::Loading), LoadingLevel::Create(LevelID::StageBoss));

	if (engine->IsKeyPressed('I'))
	{
		DropWeapon::DROP_WEAPON_DESC concealedAmmo{};
		concealedAmmo.position = _float3{ 150.f, 5.f, 170.f };
		concealedAmmo.weaponID = WeaponID::ConcealedAmmo;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DropWeapon", ENUM_CLASS(LevelID::Stage1), "Layer_DropWeapon", &concealedAmmo);

		DropWeapon::DROP_WEAPON_DESC poison{};
		poison.position = _float3{ 150.f, 5.f, 180.f };
		poison.weaponID = WeaponID::PoisionousGhost;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DropWeapon", ENUM_CLASS(LevelID::Stage1), "Layer_DropWeapon", &poison);

		DropWeapon::DROP_WEAPON_DESC prism{};
		prism.position = _float3{ 150.f, 5.f, 190.f };
		prism.weaponID = WeaponID::Prism;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DropWeapon", ENUM_CLASS(LevelID::Stage1), "Layer_DropWeapon", &prism);
	}
}

HRESULT Stage1::Render()
{
	return S_OK;
}

HRESULT Stage1::LoadMapFromFile(const _string& filePath)
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
		engine->AddObject(ENUM_CLASS(LevelID::Static), prefab.prototypeTag, ENUM_CLASS(LevelID::Stage1), prefab.layerTag, &desc);
	}
	for (auto& spawnerDesc : enemySpawnerDescs)
	{
		Object* spawner = nullptr;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemySpawner", ENUM_CLASS(LevelID::Stage1), "Layer_EnemySpawner", &spawnerDesc,&spawner);

		static_cast<EnemySpawner*>(spawner)->ConnectDoor(ENUM_CLASS(LevelID::Stage1));
	}

	/*connect door to next stage*/	
	auto lastDoor = engine->GetLastObject(ENUM_CLASS(LevelID::Stage1), "Layer_Door");
	static_cast<Door*>(lastDoor)->ConnectNextStage(LevelID::StageBoss);

	return S_OK;
}

HRESULT Stage1::Initialize_LayerLights(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun",
		ENUM_CLASS(LevelID::Stage1), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT Stage1::Initialize_LayerPlayer(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Player", ENUM_CLASS(LevelID::Static), layerTag)))
		return E_FAIL;

	engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<NavigationComponent>()->SpawnInCell(0);

	return S_OK;
}

HRESULT Stage1::Initialize_LayerUI(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sight", ENUM_CLASS(LevelID::Stage1), layerTag)))
		return E_FAIL;

	return S_OK;
}
