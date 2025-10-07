#include "MapEditorPCH.h"
#include "MapEditorPanel.h"
#include "PickingSystem.h"
#include "EngineCore.h"
#include "Layer.h"

//object
#include "PreviewObject.h"

//component
#include "MakePrefabComponent.h"
#include "ModelPickable.h"

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
			if (ImGui::Button("Change Mode"))
				m_eMode = m_eMode == EditMode::Guizmo ? EditMode::Placement : EditMode::Guizmo;

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
		
		ShowPrefabs();
	}
	ImGui::End();

	if (m_eMode == EditMode::Placement)
		Placement(state, pickRes);
	else if (m_eMode == EditMode::Guizmo)
	{
		if (pickRes.object && engine->IsMousePress(MouseButton::LButton))
			state.pObject = pickRes.object;
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
	//m_LastUsedPrefab = prefab;
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
		Object::OBJECT_DESC desc{};
		desc.position = position;
		engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), "Layer_ShowPreview", &desc);

		m_pPreviewObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_ShowPreview"]->GetFrontObject();
		m_pPreviewObject->GetComponent<ModelPickable>()->SetActive(false);
		m_pPreviewObject->SetRenderGroup(RenderGroup::Blend);
		m_pPreviewObject->AddRef();
		m_iPreviewObjectIndex = m_iSelectedIndex;
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

void MapEditorPanel::ImportMapFile(const _string& filePath)
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();

	std::vector<PREFAB> staticObjectPrefabs;
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

	for (auto& prefab : staticObjectPrefabs)
	{
		Object::OBJECT_DESC desc{};
		desc.position	= prefab.position;
		desc.scale		= prefab.scale;
		desc.quaternion = prefab.quaternion;
		engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), prefab.layerTag, &desc);
	}

	MSG_BOX("Load success");
}

void MapEditorPanel::ExportMapFile(const _string& outFilePath)
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto& layers = EngineCore::GetInstance()->GetLayers(ENUM_CLASS(LevelID::Editor));
	auto& staticMapObjects = layers["Layer_StaticMapObject"]->GetObjects();
	std::vector<PREFAB> staticObjectPrefabs;

	/*Static object prefabs*/
	for (const auto& object : staticMapObjects)
	{
		PREFAB prefab = object->GetComponent<MakePrefabComponent>()->ExportPrefab();
		staticObjectPrefabs.push_back(prefab);
	}

	ordered_json map;

	map["prefabs"] = json::array();

	/*Static object to json*/
	for (const auto& prefab : staticObjectPrefabs)
	{
		ordered_json j;
		map::PrefabToJson(j, prefab);
		map["prefabs"].push_back(std::move(j));
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
