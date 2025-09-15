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
	static ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;
	static ImGuizmo::MODE mode = ImGuizmo::WORLD;

	static _bool useSnap = true;
	static _float snapTranslate[3] = { 1.f,1.f,1.f };
	static _float snapScale[3] = { 0.1f,0.1f,0.1f };
	static _float snapRotation = 10.f;

	static _bool show = true;
	if (!show || !state.pObject)
		return;

	ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::SetOrthographic(false);

	_float4x4 view = state.pPipeLine->GetViewMatirx();
	_float4x4 proj = state.pPipeLine->GetProjMatrix();
	_float4x4 model = state.pObject->GetComponent<TransformComponent>()->GetWorldMatrix();
	
	_float colView[16];
	_float colProj[16];
	_float colModel[16];

	math::TransposeGuizmoMatrix(colView, view);
	math::TransposeGuizmoMatrix(colProj, proj);
	math::TransposeGuizmoMatrix(colModel, model);

	if (ImGuizmo::Manipulate(colView, colProj, op, mode, colModel, nullptr,
		useSnap ? snapTranslate : nullptr,
		useSnap ? &snapRotation : nullptr,
		useSnap ? snapScale : nullptr))
	{

	}

}

void GuizmoPanel::Free()
{
	__super::Free();
}

#endif