#include "EnginePCH.h"
#include "ImGuiManager.h"

#ifdef USE_IMGUI

ImGuiManager::ImGuiManager()
{
}

ImGuiManager* ImGuiManager::Create()
{
	ImGuiManager* Instance = new ImGuiManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ImGuiManager::Initialize()
{
	return S_OK;
}

void ImGuiManager::Free()
{
	__super::Free();
}

#endif // USE_IMGUI