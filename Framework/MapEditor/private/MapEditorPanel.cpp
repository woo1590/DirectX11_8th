#include "MapEditorPCH.h"
#include "MapEditorPanel.h"
#include "PickingSystem.h"
#include "EngineCore.h"
#include "Layer.h"

//object
#include "PreviewObject.h"
#include "Door.h"
#include "EnemySpawner.h"

//component
#include "MakePrefabComponent.h"
#include "ModelPickable.h"
#include "NavDataComponent.h"
#include "NavPickable.h"

MapEditorPanel::MapEditorPanel(PickingSystem* picking)
	:IPanel(),
	m_pPickingSystem(picking)
{
	m_pPickingSystem->AddRef();
}

MapEditorPanel* MapEditorPanel::Create(PickingSystem* picking)
{
	MapEditorPanel* Instance = new MapEditorPanel(picking);

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MapEditorPanel::Initialize()
{
	return S_OK;
}

void MapEditorPanel::Draw(GUIState& state)
{
	static _bool show = true;
	if (!show)
		return;

	const ImGuiViewport* vp = ImGui::GetMainViewport();

	
	auto engine = EngineCore::GetInstance();
	PICK_RESULT pickRes = m_pPickingSystem->GetLastPickResult();

	if (ImGui::Begin("Map Editor Menu", &show, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Guizmo"))
				m_eMode = EditMode::Guizmo;
			ImGui::SameLine();
			if (ImGui::Button("Placement"))
				m_eMode = EditMode::Placement;
			ImGui::SameLine();
			if (ImGui::Button("NavPlacement"))
				m_eMode = EditMode::NavPlacement;
			ImGui::SameLine();
			if (ImGui::Button("NavLink"))
				m_eMode = EditMode::NavLinked;
			ImGui::SameLine();
			if (ImGui::Button("SpawnerPlacement"))
				m_eMode = EditMode::SpawnerPlacement;

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					nfdchar_t* outPath = nullptr;
					nfdresult_t res = NFD_SaveDialog(nullptr, nullptr, &outPath);
					
					if (res == NFD_OKAY)
					{
						_string path = outPath;
						NFDi_Free(outPath);
					
						ExportMapFile(path);
					}
				}
				if (ImGui::MenuItem("Load"))
				{
					nfdchar_t* openPath = nullptr;
					nfdresult_t res = NFD_OpenDialog(nullptr, nullptr, &openPath);

					if (res == NFD_OKAY)
					{
						_string path = openPath;
						NFDi_Free(openPath);

						ImportMapFile(path);
					}
				}
				if (ImGui::MenuItem("Save Nav"))
				{
					nfdchar_t* outPath = nullptr;
					nfdresult_t res = NFD_SaveDialog(nullptr, nullptr, &outPath);

					if (res == NFD_OKAY)
					{
						_string path = outPath;
						NFDi_Free(outPath);

						ExportNavFile(path);
					}
				}
				if (ImGui::MenuItem("Load Nav"))
				{
					nfdchar_t* importPath = nullptr;
					nfdresult_t res = NFD_OpenDialog(nullptr, nullptr, &importPath);

					if (res == NFD_OKAY)
					{
						_string path = importPath;
						NFDi_Free(importPath);

						ImportNavFile(path);
					}
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();

	}
	ImGui::End();

	if (ImGui::Begin("Prefabs", &show, ImGuiWindowFlags_MenuBar))
	{
		ImGui::DragInt("Index X : ", reinterpret_cast<int*>(&pickRes.indexX));
		ImGui::DragInt("Index Z : ", reinterpret_cast<int*>(&pickRes.indexZ));
		
		ImGui::DragInt("First Nav Index : ", reinterpret_cast<int*>(&m_iFirstPickNavIndex));
		ImGui::DragInt("Second Nav Index : ", reinterpret_cast<int*>(&m_iSecondPickNavIndex));

		ShowPrefabs();
	}
	ImGui::End();

	if (m_eMode == EditMode::Placement)
		Placement(state, pickRes);
	else if (m_eMode == EditMode::NavPlacement)
		NavPlacement(state, pickRes);
	else if (m_eMode == EditMode::NavLinked)
		LinkNav(state, pickRes);
	else if (m_eMode == EditMode::SpawnerPlacement)
		SpawnerPlacement(state, pickRes);
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			if (pickRes.object && engine->IsMousePress(MouseButton::LButton))
				state.pObject = pickRes.object;
		}
	}
}

void MapEditorPanel::LoadPrefabs(const _string& filePath)
{
	auto engine = EngineCore::GetInstance();
	using json = nlohmann::json;
	namespace fs = std::filesystem;

	json j = json::parse(std::ifstream(filePath));
	
	if (j.contains("prefabs") && j["prefabs"].is_array())
	{
		PREFAB prefab{};
		for (auto& val : j["prefabs"])
		{
			prefab.prototypeTag = val.value("PrototypeTag", "");
			prefab.modelTag = val.value("ModelTag", "");
			prefab.layerTag = val.value("LayerTag", "");

			if (prefab.prototypeTag == "Prototype_Object_Door")
				engine->AddPrototype(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, Door::Create());
			else
				engine->AddPrototype(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, PreviewObject::Create(prefab, m_pPickingSystem));

			m_Prefabs.push_back(prefab);
		}
	}

	m_iNumPrefabs = m_Prefabs.size();
}

void MapEditorPanel::Free()
{
	__super::Free();
	Safe_Release(m_pPickingSystem);
	Safe_Release(m_pPreviewObject);
}

void MapEditorPanel::Placement(GUIState& state, PICK_RESULT pickRes)
{
	auto engine = EngineCore::GetInstance();

	ImGuiIO& io = ImGui::GetIO();
	if (engine->IsMousePress(MouseButton::LButton) && m_iSelectedIndex != -1 && !io.WantCaptureMouse)
		AddObjectToLayer(pickRes);
	
	if (engine->IsKeyDown('Q'))
	{
		if (engine->IsMousePress(MouseButton::RButton))
			DeleteObjectFromLayer(state, pickRes);
	}

	if (engine->IsKeyPressed('Z'))
		Undo();

	if (engine->IsKeyAway('Q'))
		ShowPreviewObject(pickRes);
}

void MapEditorPanel::AddObjectToLayer(PICK_RESULT pickRes)
{
	if (!pickRes.isHit)
		return;

	auto engine = EngineCore::GetInstance();

	PREFAB prefab = m_Prefabs[m_iSelectedIndex];
	_float3 position;
	switch (pickRes.type)
	{
	case PickType::Model:
	{
		prefab.position = pickRes.worldHitPosition;
	}break;
	case PickType::Chunk:
	{
		position.x = pickRes.chunkPosition.x - (CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexX * CELL_SIZE + CELL_SIZE * 0.5f;
		position.y = pickRes.chunkPosition.y;
		position.z = pickRes.chunkPosition.z - (CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexZ * CELL_SIZE + CELL_SIZE * 0.5f;
		prefab.position = position;
	}break;
	default:
		break;
	}

	Object::OBJECT_DESC desc{};
	desc.position = prefab.position;
	engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), prefab.layerTag, &desc);
}

void MapEditorPanel::DeleteObjectFromLayer(GUIState& state, PICK_RESULT pickRes)
{
	if (!pickRes.isHit || !pickRes.object)
		return;
	
	pickRes.object->SetDead();
	if (state.pObject == pickRes.object)
		state.pObject = nullptr;
}

void MapEditorPanel::Undo()
{
	
}

void MapEditorPanel::Redo()
{

}

void MapEditorPanel::ShowPreviewObject(PICK_RESULT pickRes)
{
	auto engine = EngineCore::GetInstance();

	_float3 position{};

	/*First set preview object*/
	if (!m_pPreviewObject && m_iSelectedIndex != -1)
	{
		if (pickRes.object)
		{
			position = pickRes.worldHitPosition;
		}
		else
		{
			position.x = pickRes.chunkPosition.x - (CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexX * CELL_SIZE + CELL_SIZE * 0.5f;
			position.y = pickRes.chunkPosition.y;
			position.z = pickRes.chunkPosition.z - (CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexZ * CELL_SIZE + CELL_SIZE * 0.5f;
		}

		PREFAB prefab = m_Prefabs[m_iSelectedIndex];

		if (prefab.prototypeTag != "Prototype_Object_Door")
		{
			Object::OBJECT_DESC desc{};
			desc.position = position;
			engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), "Layer_ShowPreview", &desc);

			m_pPreviewObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_ShowPreview"]->GetFrontObject();
			m_pPreviewObject->GetComponent<ModelPickable>()->SetActive(false);
			m_pPreviewObject->SetRenderGroup(RenderGroup::Blend);
			m_pPreviewObject->AddRef();
			m_iPreviewObjectIndex = m_iSelectedIndex;
		}
	}
	/*Alreay exist preview object*/
	else if (m_pPreviewObject)
	{
		if (m_iSelectedIndex == m_iPreviewObjectIndex)
		{
			if (pickRes.object && pickRes.object != m_pPreviewObject)
			{
				position = pickRes.worldHitPosition;
			}
			else
			{
				position.x = pickRes.chunkPosition.x - (CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexX * CELL_SIZE + CELL_SIZE * 0.5f;
				position.y = pickRes.chunkPosition.y;
				position.z = pickRes.chunkPosition.z - (CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexZ * CELL_SIZE + CELL_SIZE * 0.5f;
			}

			m_pPreviewObject->GetComponent<TransformComponent>()->SetPosition(position);
		}
		else
		{
			m_pPreviewObject->SetDead();
			Safe_Release(m_pPreviewObject);

			PREFAB prefab = m_Prefabs[m_iSelectedIndex];

			if (prefab.prototypeTag != "Prototype_Object_Door")
			{
				Object::OBJECT_DESC desc{};
				desc.position = position;
				engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), "Layer_ShowPreview", &desc);

				m_pPreviewObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_ShowPreview"]->GetLastObject();
				m_pPreviewObject->GetComponent<ModelPickable>()->SetActive(false);
				m_pPreviewObject->SetRenderGroup(RenderGroup::Blend);
				m_pPreviewObject->AddRef();
				m_iPreviewObjectIndex = m_iSelectedIndex;
			}
		}
	}
}

void MapEditorPanel::NavPlacement(GUIState& state, PICK_RESULT pickRes)
{
	if (!pickRes.isHit)
		return;

	auto engine = EngineCore::GetInstance();

	if (engine->IsMousePress(MouseButton::LButton))
	{
		if (PickType::Model ==  pickRes.type)
		{
			if (!m_isNavAdded)
			{
				_float3 position = pickRes.worldHitPosition;
				position.y += 1.f;
				m_PickPositions.push_back(position);
			}
			else if(-1 != m_iFirstPickNavIndex)
			{
				_float3 position = pickRes.worldHitPosition;
				position.y += 1.f;
				m_PickPositions.push_back(position);
			}
		}
		else if (PickType::Nav == pickRes.type)
		{
			if (-1 == m_iFirstPickNavIndex)
				m_iFirstPickNavIndex = pickRes.navCellIndex;
			else
				m_iSecondPickNavIndex = pickRes.navCellIndex;
		}

		AddNavData();
	}

	if (engine->IsKeyPressed('Z'))
		UndoNavData();
}

void MapEditorPanel::AddNavData()
{
	auto engine = EngineCore::GetInstance();
	auto navMeshObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_NavMeshObject"]->GetFrontObject();
	if (navMeshObject)
	{
		auto navData = navMeshObject->GetComponent<NavDataComponent>();

		if (!m_isNavAdded)
		{
			if (3 == m_PickPositions.size())
			{
				navData->AddNavCell(m_PickPositions.data());
				m_isNavAdded = true;
				m_PickPositions.clear();
			}
		}
		else
		{
			if (-1 != m_iFirstPickNavIndex && 1 == m_PickPositions.size())
			{
				navData->AddNavCell(m_iFirstPickNavIndex, m_PickPositions[0]);
				m_iFirstPickNavIndex = -1;
				m_PickPositions.clear();
			}
			else if (-1 != m_iFirstPickNavIndex && -1 != m_iSecondPickNavIndex)
			{
				if (m_iFirstPickNavIndex != m_iSecondPickNavIndex)
					navData->AddNavCell(m_iFirstPickNavIndex, m_iSecondPickNavIndex);
				

				m_iFirstPickNavIndex = -1;
				m_iSecondPickNavIndex = -1;
			}
		}
	}
}

void MapEditorPanel::UndoNavData()
{
	auto engine = EngineCore::GetInstance();
	auto navMeshObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_NavMeshObject"]->GetFrontObject();
	if (navMeshObject)
	{
		auto navData = navMeshObject->GetComponent<NavDataComponent>();
		navData->ReomoveLastCell();
	}
}

void MapEditorPanel::LinkNav(GUIState& state, PICK_RESULT pickRes)
{
	auto engine = EngineCore::GetInstance();
	auto navMeshObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_NavMeshObject"]->GetFrontObject();
	if (PickType::Nav == pickRes.type)
	{
		if (engine->IsMousePress(MouseButton::LButton))
		{
			if (-1 == m_iFirstPickNavIndex)
				m_iFirstPickNavIndex = pickRes.navCellIndex;
			else
				m_iSecondPickNavIndex = pickRes.navCellIndex;
		}

	}

	if (-1 != m_iFirstPickNavIndex && -1 != m_iSecondPickNavIndex)
	{
		navMeshObject->GetComponent<NavDataComponent>()->LinkCell(m_iFirstPickNavIndex, m_iSecondPickNavIndex);
		m_iFirstPickNavIndex = -1;
		m_iSecondPickNavIndex = -1;
	}
}

void MapEditorPanel::SpawnerPlacement(GUIState& state, PICK_RESULT pickRes)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;

	auto engine = EngineCore::GetInstance();

	switch (pickRes.type)
	{
	case PickType::Spawner:
	{
		m_pEnemySpawner = pickRes.object;
	}break;
	case PickType::Model:
	{
		if (engine->IsMousePress(MouseButton::LButton))
			AddSpawner(pickRes);
	}break;
	case PickType::Nav:
	{
		if (m_pEnemySpawner && engine->IsMousePress(MouseButton::LButton))
			AddNavCellToSpawner(pickRes.navCellIndex);
	}break;
	default:
		break;
	}	
}

void MapEditorPanel::AddSpawner(PICK_RESULT pickRes)
{
	if (!pickRes.isHit || !pickRes.object)
		return;

	auto engine = EngineCore::GetInstance();

	Object::OBJECT_DESC desc{};
	desc.position = pickRes.worldHitPosition;
	engine->AddObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_EnemySpawner", ENUM_CLASS(LevelID::Editor), "Layer_EnemySpawner", &desc);

}

void MapEditorPanel::AddNavCellToSpawner(_uint index)
{
	auto spawner = static_cast<EnemySpawner*>(m_pEnemySpawner);

	spawner->AddNavCellIndex(index);
}

void MapEditorPanel::ImportMapFile(const _string& filePath)
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();

	std::vector<PREFAB> staticObjectPrefabs;
	std::vector<EnemySpawner::ENEMY_SPAWNER_DESC> spawnerDescs;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load");
		return;
	}

	ordered_json map = json::parse(file);

	/*Load Static obeject prefab*/
	for (auto& j : map["prefabs"])
	{
		PREFAB prefab{};
		map::PrefabFromJson(j, prefab);
		staticObjectPrefabs.push_back(prefab);
	}
	/*Load Spawners*/
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

		desc.navCellIndices = j.value("NavCells", std::vector<_uint>{});

		spawnerDescs.push_back(desc);
	}

	for (auto& prefab : staticObjectPrefabs)
	{
		Object::OBJECT_DESC desc{};
		desc.position	= prefab.position;
		desc.scale		= prefab.scale;
		desc.quaternion = prefab.quaternion;
		engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), prefab.layerTag, &desc);
	}
	for (auto& desc : spawnerDescs)
		engine->AddObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_EnemySpawner", ENUM_CLASS(LevelID::Editor), "Layer_EnemySpawner", &desc);

	MSG_BOX("Load success");
}

void MapEditorPanel::ExportMapFile(const _string& outFilePath)
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto& layers = EngineCore::GetInstance()->GetLayers(ENUM_CLASS(LevelID::Editor));
	auto& staticMapObjects = layers["Layer_StaticMapObject"]->GetObjects();
	auto& doors = layers["Layer_Door"]->GetObjects();
	auto& enemySpawners = layers["Layer_EnemySpawner"]->GetObjects();

	std::vector<PREFAB> objectPrefabs;
	std::vector<EnemySpawner::ENEMY_SPAWNER_DESC> enemySpawnerDescs;
	/*Static object prefabs*/
	for (const auto& object : staticMapObjects)
	{
		PREFAB prefab = object->GetComponent<MakePrefabComponent>()->ExportPrefab();
		objectPrefabs.push_back(prefab);
	}
	/*Door prefabs*/
	for (const auto& object : doors)
	{
		PREFAB prefab = object->GetComponent<MakePrefabComponent>()->ExportPrefab();
		objectPrefabs.push_back(prefab);
	}
	/*Spanwer desc*/
	for(const auto& spawner : enemySpawners)
	{
		EnemySpawner::ENEMY_SPAWNER_DESC desc = static_cast<EnemySpawner*>(spawner)->ExportDesc();
		enemySpawnerDescs.push_back(desc);
	}

	ordered_json map;
	map["prefabs"] = json::array();
	map["enemyspawners"] = json::array();

	/*Static object to json*/
	for (const auto& prefab : objectPrefabs)
	{
		ordered_json j;
		map::PrefabToJson(j, prefab);
		map["prefabs"].push_back(std::move(j));
	}
	/*Spawners to json*/
	for (const auto& desc : enemySpawnerDescs)
	{
		ordered_json j;
		j = ordered_json
		{
			{ "PrototypeTag", "Prototype_Object_EnemySpawner" },
			{ "ModelTag",		""},
			{ "LayerTag",		"Layer_EnemySpawner"},
			{ "Position",		{{"x",desc.position.x},{"y",desc.position.y},{"z",desc.position.z}} },
			{ "Scale",			{{"x",desc.scale.x},{"y",desc.scale.y},{"z",desc.scale.z}} },
			{ "Quaternion",		{{"x",desc.quaternion.x},{"y",desc.quaternion.y},{"z",desc.quaternion.z},{"w",desc.quaternion.w}} },
			{"NavCells",desc.navCellIndices}
		};
		map["enemyspawners"].push_back(std::move(j));
	}

	std::ofstream out(outFilePath);
	if (!out.is_open())
	{
		MSG_BOX("Failed to save");
		return;
	}
	out << map.dump(2);

	MSG_BOX("Save Success!");
}

void MapEditorPanel::ImportNavFile(const _string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load");
		return;
	}

	auto engine = EngineCore::GetInstance();
	auto navMeshObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_NavMeshObject"]->GetFrontObject();

	navMeshObject->GetComponent<NavDataComponent>()->ImportNavData(file);

	m_isNavAdded = true;
	m_PickPositions.clear();
	m_iFirstPickNavIndex = -1;
	m_iSecondPickNavIndex = -1;
}

void MapEditorPanel::ExportNavFile(const _string& filePath)
{
	std::ofstream out(filePath, std::ios::binary);
	if (!out.is_open())
	{
		MSG_BOX("Failed to save");
		return;
	}

	auto engine = EngineCore::GetInstance();
	auto navMeshObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_NavMeshObject"]->GetFrontObject();

	navMeshObject->GetComponent<NavDataComponent>()->ExportNavData(out);
}

void MapEditorPanel::ShowPrefabs()
{
	/*Prefab is selected!*/
	if (m_iSelectedIndex != -1)
	{
		ImGui::SeparatorText("Selected Prefab");
		ImGui::Text(m_Prefabs[m_iSelectedIndex].prototypeTag.c_str());
	}

	ImGui::SeparatorText("Prototype Tag");
	for (_uint i = 0; i < m_iNumPrefabs; ++i)
	{
		if (ImGui::Selectable(m_Prefabs[i].prototypeTag.c_str()))
		{
			m_iSelectedIndex = i;
		}
	}
}
