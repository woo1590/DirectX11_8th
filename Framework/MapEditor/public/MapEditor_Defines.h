#pragma once

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"
#endif

namespace MapEditor
{
	constexpr unsigned int WinSizeX = 1280;
	constexpr unsigned int WinSizeY = 720;

	constexpr unsigned int CHUNK_SIZE = 256;
	constexpr unsigned int CELL_SIZE = 40;

	enum class LevelID { Editor, Count };
}