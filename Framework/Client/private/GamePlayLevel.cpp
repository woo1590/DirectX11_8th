#include "pch.h"
#include "GamePlayLevel.h"
#include "EngineCore.h"
#include "Object.h"

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

	if (engine->IsKeyPressed('0'))
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss", ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy");
	if (engine->IsKeyPressed('9'))
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Bomber", ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy");
	if (engine->IsKeyPressed('8'))
		engine->AddObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Soldier", ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy");
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
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load");
		return E_FAIL;
	}

	ordered_json map = json::parse(file);

	/*Load Static obeject prefab*/
	for (auto& j : map["prefabs"])
	{
		PREFAB prefab{};
		map::PrefabFromJson(j, prefab);
		staticObjectPrefabs.push_back(prefab);
	}
	/*Load Enemy Spawners*/
	for (auto& j : map["spawners"])
	{

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

	player->GetComponent<NavigationComponent>()->SpawnInCell(0);

	return S_OK;
}

HRESULT GamePlayLevel::Initialize_LayerEnemy(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	return S_OK;
}
