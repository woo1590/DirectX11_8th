#pragma once

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"
#endif

namespace Client
{
	constexpr unsigned int WinSizeX = 1280;
	constexpr unsigned int WinSizeY = 720;

	enum class LevelID { Static, Loading, Logo, GamePlay, Test, Count };

}
