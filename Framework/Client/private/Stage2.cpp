#include "pch.h"
#include "Stage2.h"
#include "Door.h"
#include "EnemySpawner.h"
#include "PointLight.h"
#include "Torch.h"
#include "NavigationComponent.h"
#include "SkillPanel.h"
#include "GameManager.h"
#include <StageFont.h>
#include <Chest.h>
#include <Command_ChangeLevel.h>
#include <LoadingLevel.h>

Stage2::Stage2()
	:Level()
{
}

Stage2* Stage2::Create()
{
	Stage2* Instance = new Stage2();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Stage2::Initialize()
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(LoadMapFromFile("../bin/data/map/stage2_map_spawn.json")))
		return E_FAIL;

	if (FAILED(LoadLightFromFile("../bin/data/map/stage2_map_light.json")))
		return E_FAIL;

	engine->SetNavMesh(ENUM_CLASS(LevelID::Static), "NavMesh_Stage2");

	if (FAILED(Initialize_LayerUI("Layer_UI")))
		return E_FAIL;

	if (FAILED(Initialize_LayerPlayer("Layer_Player")))
		return E_FAIL;

	if (FAILED(Initialize_LayerCamera("Layer_Camera")))
		return E_FAIL;

	if (FAILED(Initialize_LayerChest("Layer_Chest")))
		return E_FAIL;

	if (FAILED(Initialize_LayerBarrel("Layer_Barrel")))
		return E_FAIL;

	return S_OK;
}

void Stage2::Free()
{
	__super::Free();
}

void Stage2::Update(_float dt)
{
	if (EngineCore::GetInstance()->IsKeyPressed('O'))
	{
		EngineCore::GetInstance()->RegisterCommand(Command_ChangeLevel::Create(LevelID::Loading, LoadingLevel::Create(LevelID::Stage2)));
	}
}

HRESULT Stage2::Render()
{
	return S_OK;
}

HRESULT Stage2::LoadMapFromFile(const _string& filePath)
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
		engine->AddObject(ENUM_CLASS(LevelID::Static), prefab.prototypeTag, ENUM_CLASS(LevelID::Stage2), prefab.layerTag, &desc);
	}
	for (auto& spawnerDesc : enemySpawnerDescs)
	{
		Object* spawner = nullptr;
		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemySpawner", ENUM_CLASS(LevelID::Stage2), "Layer_EnemySpawner", &spawnerDesc, &spawner);

		static_cast<EnemySpawner*>(spawner)->ConnectDoor(ENUM_CLASS(LevelID::Stage2));
	}

	/*connect door to next stage*/
	auto lastDoor = engine->GetLastObject(ENUM_CLASS(LevelID::Stage2), "Layer_Door");
	static_cast<Door*>(lastDoor)->ConnectNextStage(LevelID::Stage3);

	return S_OK;
}

HRESULT Stage2::LoadLightFromFile(const _string& filePath)
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

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::Stage2), "Layer_PointLight", &desc);

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

		engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Torch", ENUM_CLASS(LevelID::Stage2), "Layer_Torch", &desc);
	}

	return S_OK;
}

HRESULT Stage2::Initialize_LayerPlayer(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
	player->GetComponent<NavigationComponent>()->SpawnInCell(2);

	return S_OK;
}

HRESULT Stage2::Initialize_LayerCamera(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_ShadowCam", ENUM_CLASS(LevelID::Stage2), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT Stage2::Initialize_LayerUI(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	Object* playerPanel = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerPanel", ENUM_CLASS(LevelID::Stage2), layerTag, nullptr, &playerPanel)))
		return E_FAIL;

	Object* weaponPanel = nullptr;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponPanel", ENUM_CLASS(LevelID::Stage2), layerTag, nullptr, &weaponPanel)))
		return E_FAIL;

	Object* skillPanel = nullptr;
	SkillPanel::SKILL_PANEL_DESC skillPanelDesc{};
	skillPanelDesc.numPanel = 0;
	skillPanelDesc.skillID = SkillID::Dash;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SkillPanel", ENUM_CLASS(LevelID::Stage2), layerTag, &skillPanelDesc, &weaponPanel)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectBackground", ENUM_CLASS(LevelID::Stage2), layerTag)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitCrossHair", ENUM_CLASS(LevelID::Stage2), layerTag)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_InteractionUI", ENUM_CLASS(LevelID::Stage2), layerTag)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapPanel", ENUM_CLASS(LevelID::Stage2), layerTag)))
		return E_FAIL;

	StageFont::STAGE_FONT_DESC desc{};
	desc.stageID = LevelID::Stage2;
	desc.position = _float3{ -879.f,22.f, 72.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_StageFont", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT Stage2::Initialize_LayerChest(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	{
		Object* chest = nullptr;
		Chest::CHEST_DESC chest1Desc{};
		chest1Desc.weaponID = WeaponID::Cameleon;
		chest1Desc.position = _float3{ -670.f,-5.f,164.f };
		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", ENUM_CLASS(LevelID::Stage2), layerTag, &chest1Desc, &chest)))
			return E_FAIL;
	}
	
	{
		Object* chest = nullptr;
		Chest::CHEST_DESC chest1Desc{};
		chest1Desc.weaponID = WeaponID::Prism;
		chest1Desc.position = _float3{ -320.f,-4.f,146.f };
		if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", ENUM_CLASS(LevelID::Stage2), layerTag, &chest1Desc, &chest)))
			return E_FAIL;

		chest->GetComponent<TransformComponent>()->Rotate(_float3{ 0.f,math::ToRadian(90.f),0.f });
	}

	return S_OK;
}

HRESULT Stage2::Initialize_LayerBarrel(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	Object::OBJECT_DESC desc{};
	desc.position = _float3{ -750.f,-13.f,120.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -726.f,-13.f,120.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -392.f,-4.f,88.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ 750.f,-14.5f,120.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -454.f,-4.5f,170.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	desc.position = _float3{ -40.f,-6.5f,140.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;
	
	desc.position = _float3{ -152.f,5.f,90.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", ENUM_CLASS(LevelID::Stage2), layerTag, &desc)))
		return E_FAIL;

	return S_OK;
}
