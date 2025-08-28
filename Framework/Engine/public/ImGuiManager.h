#pragma once
#include "Base.h"

#ifdef USE_IMGUI

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"

NS_BEGIN(Engine)

class ImGuiManager :
    public Base
{
private:
    ImGuiManager();
    virtual ~ImGuiManager() = default;

public:
    static ImGuiManager * Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    HRESULT Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    _bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void IMGUI_TEST_FUNC();
    void Free()override;

private:

};

NS_END

#endif