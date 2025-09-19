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

	if (ImGui::Begin("Map Editor", &show, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("Map Editor Test");
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void MapEditorPanel::Free()
{
	__super::Free();
}
