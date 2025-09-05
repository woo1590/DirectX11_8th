#include "EnginePCH.h"
#include "ImGuiManager.h"
#include "EngineCore.h"
#include "OutlinerPanel.h"
#include "InspectorPanel.h"

#ifdef USE_IMGUI

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImGuiManager::ImGuiManager()
{
}

ImGuiManager * ImGuiManager::Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GUIState state)
{
	ImGuiManager* Instance = new ImGuiManager();

	if (FAILED(Instance->Initialize(hWnd, pDevice, pContext, state)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ImGuiManager::Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GUIState state)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SetupIO();

	if (!ImGui_ImplWin32_Init(hWnd))
		return E_FAIL;

	if (!ImGui_ImplDX11_Init(pDevice,
							 pContext))
		return E_FAIL;

	auto outliner = OutlinerPanel::Create();
	if (!outliner)
		return E_FAIL;

	auto inspector = InspectorPanel::Create();
	if (!inspector)
		return E_FAIL;

	m_Panels.push_back(outliner);
	m_Panels.push_back(inspector);

	m_GuiState = state;

	return S_OK;
}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::Render()
{
	for (const auto& panel : m_Panels)
	{
		panel->Draw(m_GuiState);
	}
}

void ImGuiManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

_bool ImGuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiManager::Free()
{
	__super::Free();

	for (auto& panel : m_Panels)
		Safe_Release(panel);
	m_Panels.clear();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::SetupIO()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	//io.Fonts->AddFontFromFileTTF("../public/imgui/imgui_font/DungGeunMo.ttf", 18.f);
}

#endif // USE_IMGUI