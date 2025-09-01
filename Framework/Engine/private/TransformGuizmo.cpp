#include "EnginePCH.h"
#include "TransformGuizmo.h"

#ifdef USE_IMGUI

TransformGuizmo::TransformGuizmo()
{
}

TransformGuizmo* TransformGuizmo::Create()
{
	TransformGuizmo* Instance = new TransformGuizmo();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TransformGuizmo::Initialize()
{
	return S_OK;
}

void TransformGuizmo::Free()
{
	__super::Free();
}

#endif