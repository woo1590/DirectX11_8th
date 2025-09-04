#pragma once
#include "Base.h"

#ifdef USE_IMGUI

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"

NS_BEGIN(Engine)

class IPanel :
    public Base
{
protected:
    IPanel();
    virtual ~IPanel() = default;

public:
    virtual void Draw() = 0;

private:

};

NS_END

#endif