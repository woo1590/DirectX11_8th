#include "EnginePCH.h"
#include "GuizmoPanel.h"
#include "PipeLine.h"
#include "Object.h"
#include "TransformComponent.h"

#ifdef USE_IMGUI

GuizmoPanel::GuizmoPanel()
{
}

GuizmoPanel* GuizmoPanel::Create()
{
	GuizmoPanel* Instance = new GuizmoPanel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT GuizmoPanel::Initialize()
{
	
	return S_OK;
}

void GuizmoPanel::Draw(GUIState& state)
{
	static _bool show = true;
	if (!show)
		return;

	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();
	ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
	ImGuizmo::SetOrthographic(false);



	if (ImGui::Begin("Guizmo"))
	{
		
	}

	ImGui::End();
}

void GuizmoPanel::Free()
{
	__super::Free();
}

#endif