#include "pch.h"
#include "Stage1.h"
#include "EngineCore.h"
#include "LoadingLevel.h"
#include "Command_ChangeLevel.h"
#include "EffectContainer.h"

//object
#include "Door.h"
#include "EnemySpawner.h"
#include "DropWeapon.h"
#include "Chest.h"
#include "PointLight.h"
#include "Torch.h"

#include "Player.h"
#include "WeaponPanel.h"
#include "PlayerPanel.h"
#include "SkillPanel.h"

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

	if (FAILED(LoadLightFromFile("../bin/data/map/stage1_map_light.json")))
		return E_FAIL;

	engine->SetNavMesh(ENUM_CLASS(LevelID::Static), "NavMesh_Stage1");

	if (FAILED(Initialize_LayerUI("Layer_UI")))
		return E_FAIL;

	if (FAILED(Initialize_LayerPlayer("Layer_Player")))
		return E_FAIL;

	if (FAILED(Initialize_LayerChest("Layer_Chest")))
		return E_FAIL;

	m_iBGMChannelId = engine->Play2DSound("BGM_Stage1", 0.1f);

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
	{
		//engine->IsNavDebugEnable() ? engine->NavDebugDisable() : engine->NavDebugEnable();
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_HorseHead", ENUM_CLASS(LevelID::Stage1), "Layer_Enemy");
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpearMan", ENUM_CLASS(LevelID::Stage1), "Layer_Enemy");
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Soldier", ENUM_CLASS(LevelID::Stage1), "Layer_Enemy");
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_CrossbowMan", ENUM_CLASS(LevelID::Stage1), "Layer_Enemy");
	}

	if (engine->IsKeyPressed('O'))
	{
		engine->RegisterCommand(Command_ChangeLevel::Create(LevelID::StageBoss));
	}

	m_fSFXElapsedTime += dt;
	if (m_fSFXElapsedTime >= 30.f)
	{
		engine->Play2DSound("SFX_Stage");
		m_fSFXElapsedTime = 0.f;
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

HRESULT Stage1::LoadLightFromFile(const _string& filePath)
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

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::Stage1), "Layer_PointLight", &desc);

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


HRESULT Stage1::Initialize_LayerPlayer(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Player", ENUM_CLASS(LevelID::Static), layerTag)))
		return E_FAIL;

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	static_cast<Player*>(player)->GetComponent<NavigationComponent>()->SpawnInCell(0);

	return S_OK;
}

HRESULT Stage1::Initialize_LayerUI(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	Object* playerPanel = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerPanel", ENUM_CLASS(LevelID::Stage1), layerTag, nullptr, &playerPanel)))
		return E_FAIL;
	
	Object* weaponPanel = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponPanel", ENUM_CLASS(LevelID::Stage1), layerTag, nullptr, &weaponPanel)))
		return E_FAIL;

	Object* skillPanel = nullptr;
	SkillPanel::SKILL_PANEL_DESC skillPanelDesc{};
	skillPanelDesc.numPanel = 0;
	skillPanelDesc.skillID = SkillID::Dash;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SkillPanel", ENUM_CLASS(LevelID::Stage1), layerTag, &skillPanelDesc, &weaponPanel)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectBackground", ENUM_CLASS(LevelID::Stage1), layerTag)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitCrossHair", ENUM_CLASS(LevelID::Stage1), layerTag)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_InteractionUI", ENUM_CLASS(LevelID::Stage1), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT Stage1::Initialize_LayerChest(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	Chest::CHEST_DESC chest1Desc{};
	chest1Desc.weaponID = WeaponID::Prism;
	chest1Desc.position = _float3{ 40.f,0.f,200.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", ENUM_CLASS(LevelID::Stage1), layerTag, &chest1Desc)))
		return E_FAIL;

	Chest::CHEST_DESC chest2Desc{};
	chest2Desc.weaponID = WeaponID::Cameleon;
	chest2Desc.position = _float3{ 40.f,0.f,250.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", ENUM_CLASS(LevelID::Stage1), layerTag, &chest2Desc)))
		return E_FAIL;

	Chest::CHEST_DESC chest3Desc{};
	chest3Desc.weaponID = WeaponID::ConcealedAmmo;
	chest3Desc.position = _float3{ 40.f,0.f,150.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", ENUM_CLASS(LevelID::Stage1), layerTag, &chest3Desc)))
		return E_FAIL;

	return S_OK;
}
