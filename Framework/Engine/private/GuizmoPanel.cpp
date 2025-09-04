#include "EnginePCH.h"
#include "GuizmoPanel.h"

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