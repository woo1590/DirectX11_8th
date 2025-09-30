#include "MapEditorPCH.h"
#include "MapEditorPanel.h"
#include "PickingSystem.h"
#include "EngineCore.h"
#include "PreviewObject.h"
#include "Layer.h"

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
	PICK_RESULT pickRes = m_pPickingSystem->GetLastPickResult();

	if (ImGui::Begin("Map Editor Menu", &show, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{

				}
				if (ImGui::MenuItem("Load"))
				{

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

	KeyInput(pickRes);
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
}

void MapEditorPanel::KeyInput(PICK_RESULT pickRes)
{
	
	auto engine = EngineCore::GetInstance();

	ImGuiIO& io = ImGui::GetIO();
	if (engine->IsMousePress(MouseButton::LButton) && m_iSelectedIndex != -1 && !io.WantCaptureMouse)
		AddObjectToLayer(pickRes);
	if (engine->IsMousePress(MouseButton::RButton))
		DeleteObjectFromLayer(pickRes);
	if (engine->IsKeyPressed('Z'))
		Undo();

	//ShowPreviewObject(pickRes);
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
		prefab.position = pickRes.hitPosition;
	}break;
	case PickType::Chunk:
	{
		position.x = -(CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexX * CELL_SIZE + CELL_SIZE * 0.5f;
		position.y = 0.f;
		position.z = -(CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexZ * CELL_SIZE + CELL_SIZE * 0.5f;
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

void MapEditorPanel::DeleteObjectFromLayer(PICK_RESULT pickRes)
{
	if (!pickRes.isHit || !pickRes.object)
		return;
	
	pickRes.object->SetDead();
}

void MapEditorPanel::Undo()
{
	
}

void MapEditorPanel::Redo()
{

}

void MapEditorPanel::ShowPreviewObject(PICK_RESULT pickRes)
{
	/*auto engine = EngineCore::GetInstance();

	_float3 position{};
	if (!m_pPreviewObject && m_iSelectedIndex != -1)
	{
		if (pickRes.object)
		{
			XMStoreFloat3(&position, XMLoadFloat3(&pickRes.origin) + XMVector3Normalize(XMLoadFloat3(&pickRes.direction)) * XMLoadFloat(&pickRes.distance));
		}
		else
		{
			position.x = -(CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexX * CELL_SIZE + CELL_SIZE * 0.5f;
			position.y = 0.f;
			position.z = -(CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexZ * CELL_SIZE + CELL_SIZE * 0.5f;
		}

		PREFAB prefab = m_Prefabs[m_iSelectedIndex];
		Object::OBJECT_DESC desc{};
		desc.position = position;
		engine->AddObject(ENUM_CLASS(LevelID::Editor), prefab.prototypeTag, ENUM_CLASS(LevelID::Editor), "Layer_ShowPreview", &desc);

		m_pPreviewObject = engine->GetLayers(ENUM_CLASS(LevelID::Editor))["Layer_ShowPreview"]->GetFrontObject();
	}
	else if (m_pPreviewObject)
	{
		if (pickRes.object)
		{
			XMStoreFloat3(&position, XMLoadFloat3(&pickRes.origin) + XMVector3Normalize(XMLoadFloat3(&pickRes.direction)) * XMLoadFloat(&pickRes.distance));
		}
		else
		{
			position.x = -(CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexX * CELL_SIZE + CELL_SIZE * 0.5f;
			position.y = 0.f;
			position.z = -(CHUNK_SIZE * 0.5f * CELL_SIZE) + pickRes.indexZ * CELL_SIZE + CELL_SIZE * 0.5f;
		}

		m_pPreviewObject->GetComponent<TransformComponent>()->SetPosition(position);
	}*/
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
