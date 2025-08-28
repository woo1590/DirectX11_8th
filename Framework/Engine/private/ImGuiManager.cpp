#include "EnginePCH.h"
#include "ImGuiManager.h"
#include "EngineCore.h"

#ifdef USE_IMGUI

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImGuiManager::ImGuiManager()
{
}

ImGuiManager * ImGuiManager::Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ImGuiManager* Instance = new ImGuiManager();

	if (FAILED(Instance->Initialize(hWnd, pDevice, pContext)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ImGuiManager::Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	if (!ImGui_ImplWin32_Init(hWnd))
		return E_FAIL;

	if (!ImGui_ImplDX11_Init(pDevice,
							 pContext))
		return E_FAIL;

	return S_OK;
}

_bool ImGuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiManager::IMGUI_TEST_FUNC()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("DX11 + Win32");
	ImGui::Text("Test Widget");
	static _float f = 0.f;
	ImGui::SliderFloat("float", &f, 0.f, 1.f);
	if(ImGui::Button("Button"))
	{ }
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Free()
{
	__super::Free();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

#endif // USE_IMGUI