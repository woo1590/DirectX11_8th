#include "EnginePCH.h"
#include "ImGuiManager.h"
#include "EngineCore.h"
#include "OutlinerPanel.h"
#include "InspectorPanel.h"
#include "GuizmoPanel.h"

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

	SetUpIO();
    SetUpStyle();

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

	auto guizmo = GuizmoPanel::Create();
	if (!guizmo)
		return E_FAIL;

	m_Panels.push_back(outliner);
	m_Panels.push_back(inspector);
	m_Panels.push_back(guizmo);

	m_GuiState = state;

	return S_OK;
}

void ImGuiManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiManager::Render()
{
    auto engine = EngineCore::GetInstance();

	ImGuiIO& io = ImGui::GetIO();
    static _bool isAvailable = true;
	ImGui::Begin("Debug");
	ImGui::Text("FPS : %.1f (%.3f ms)", io.Framerate, 1000.f / io.Framerate);
    if (ImGui::Button("Visible"))
        isAvailable = isAvailable ? false : true;
    if (ImGui::Button("Debug Mode"))
        engine->IsDebugEnable() ? engine->DebugDisable() : engine->DebugEnable();
	ImGui::End();


    if (isAvailable)
    {
	    for (const auto& panel : m_Panels)
	    {
		    panel->Draw(m_GuiState);
	    }
    }
}

void ImGuiManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::AddPanel(IPanel* panel)
{
	m_Panels.push_back(panel);
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

void ImGuiManager::SetUpIO()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("../../EngineSDK/inc/imgui/imgui_font/suite.ttf", 18.f);
}

void ImGuiManager::SetUpStyle()
{
    _float alpha_ = 0.9f;
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.FrameRounding = 3.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    //style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    //style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    //style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    //style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    //style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
    //style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    //style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    //style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    for (int i = 0; i <= ImGuiCol_COUNT; i++)
    {
        ImVec4& col = style.Colors[i];
        float H, S, V;
        ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

        if (S < 0.1f)
        {
            V = 1.0f - V;
        }
        ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
        if (col.w < 1.00f)
        {
            col.w *= alpha_;
        }
    }

}

void ImGuiManager::SetUpDock()
{

}

#endif // USE_IMGUI