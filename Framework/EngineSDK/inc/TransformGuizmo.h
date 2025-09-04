#pragma once
#include "Base.h"

#ifdef USE_IMGUI

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"

NS_BEGIN(Engine)

class TransformComponent;
class ImGuiManager;
class TransformGuizmo final:
    public Base
{
private:
    TransformGuizmo();
    virtual ~TransformGuizmo() = default;

public:
    static TransformGuizmo* Create();
    HRESULT Initialize();

    void RegisterGuizmo(ImGuiManager* pImGuiManager);
    void Free()override;

private:
    TransformComponent* m_pTargetTransform = nullptr;

};

NS_END

#endif