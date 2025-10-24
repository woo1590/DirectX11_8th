#include "EnginePCH.h"
#include "GuizmoPanel.h"
#include "CameraManager.h"
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
	auto engine = EngineCore::GetInstance();

	static ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE | ImGuizmo::ROTATE;
	static ImGuizmo::MODE mode = ImGuizmo::WORLD;
	D3D11_VIEWPORT viewPort{};
	_uint num = 1;
	engine->GetDeviceContext()->RSGetViewports(&num, &viewPort);

	static _bool useSnap = true;
	static _float snapTranslate[3] = { 1.f,1.f,1.f };
	static _float snapScale[3] = { 0.1f,0.1f,0.1f };
	static _float snapRotation = 10.f;

	static _bool show = true;
	if (!show || !state.pObject)
		return;

	ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
	ImGuizmo::SetRect(viewPort.TopLeftX, viewPort.TopLeftY, viewPort.Width, viewPort.Height);
	ImGuizmo::SetOrthographic(false);

	CAMERA_CONTEXT camContext = state.pCameraManager->GetCameraContext();
	auto transform = state.pObject->GetComponent<TransformComponent>();
	_float4x4 view = camContext.viewMatrix;
	_float4x4 proj = camContext.projMatrix;
	_float4x4 model = transform->GetWorldMatrix();
	
	_float colView[16];
	_float colProj[16];
	_float colModel[16];

	math::TransposeGuizmoMatrix(colView, view);
	math::TransposeGuizmoMatrix(colProj, proj);
	math::TransposeGuizmoMatrix(colModel, model);

	const _float* snapVec3 = nullptr;
	const _float* snapRot = nullptr;
	
	if (useSnap)
	{
		if (op == ImGuizmo::TRANSLATE) snapVec3 = snapTranslate;
		if (op == ImGuizmo::SCALE) snapVec3 = snapScale;
		if (op == ImGuizmo::ROTATE) snapRot = &snapRotation;
	}

	if (ImGuizmo::Manipulate(colView, colProj, op, mode,
		colModel, nullptr,
		snapVec3,
		snapRot))
	{
		_vector positionV, scaleV, quternionV;
		_float4x4 worldMat{};
		_matrix colWorldMat{};
		_matrix rowWolrdMat{};

		memcpy_s(&worldMat, sizeof(_float4x4), colModel, sizeof(_float4x4));
		colWorldMat = XMLoadFloat4x4(&worldMat);
		rowWolrdMat = XMMatrixTranspose(colWorldMat);
		XMStoreFloat4x4(&worldMat, colWorldMat);

		if (XMMatrixDecompose(&scaleV, &quternionV, &positionV, XMLoadFloat4x4(&worldMat)))
		{
			_float3 position, scale;
			_float4 quaternion;

			XMStoreFloat3(&position, positionV);
			XMStoreFloat3(&scale, scaleV);
			XMStoreFloat4(&quaternion,quternionV);

			transform->SetPosition(position);
			transform->SetScale(scale);
			transform->SetQuaternion(quaternion);
		}
	}
}

void GuizmoPanel::Free()
{
	__super::Free();
}

#endif