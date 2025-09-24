#include "MapEditorPCH.h"
#include "MapEditorPanel.h"

MapEditorPanel::MapEditorPanel()
	:IPanel()
{
}

MapEditorPanel* MapEditorPanel::Create()
{
	MapEditorPanel* Instance = new MapEditorPanel();

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
		if (ImGui::BeginTable("MapEditorLayout", 2,
			ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthFixed, 320.0f);
			ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ShowPrefabs();

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("test2");

			ImGui::EndTable();
		}
	}
	ImGui::End();
}

void MapEditorPanel::Free()
{
	__super::Free();
}

void MapEditorPanel::AddPrefab()
{
	PREFAB prefab{};

	static _char buffer[128];

	ImGui::TextUnformatted("Input Prototype Tag..");
	if (ImGui::InputText("##prototype_tag", buffer, 128,
		ImGuiInputTextFlags_AutoSelectAll |
		ImGuiInputTextFlags_EnterReturnsTrue))
	{
		prefab.prototypeTag = buffer;
	}

	if (ImGui::Button("Add"))
	{
		m_Prefabs.push_back(prefab);
		++m_iNumPrefabs;
	}
}

void MapEditorPanel::ShowPrefabs()
{
	for (_uint i = 0; i < m_iNumPrefabs; ++i)
	{
		ImGui::SeparatorText("Prototype Tag");
		ImGui::Text(m_Prefabs[i].prototypeTag.c_str());
	}
}
