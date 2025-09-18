#pragma once
#include "Base.h"

#ifdef USE_IMGUI

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/IconsFontAwesome7.h"
#include "guizmo/ImGuizmo.h"
#include "GUIState.h"

NS_BEGIN(Engine)

class IPanel;
class ImGuiManager :
    public Base
{
private:
    ImGuiManager();
    virtual ~ImGuiManager() = default;

public:
    static ImGuiManager * Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GUIState state);
    HRESULT Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GUIState state);
    
    void BeginFrame();
    void Render();
    void EndFrame();

    void AddPanel(IPanel* panel);
    _bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void Free()override;

private:
    void SetUpIO();
    void SetUpStyle();

    std::vector<IPanel*> m_Panels;
    GUIState m_GuiState{};
};

NS_END

#endif