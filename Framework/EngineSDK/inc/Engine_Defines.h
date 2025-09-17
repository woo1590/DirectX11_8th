#pragma once

#define USE_IMGUI
#pragma warning (disable : 4005)
#pragma warning (disable : 4251)



//d3d
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "fx11/d3dx11effect.h"
#include "directxtk/DDSTextureLoader.h"
#include "directxtk/WICTextureLoader.h"
#include "directxtk/ScreenGrab.h"

#include <d3dcompiler.h>
using namespace DirectX;

//library
#include "fmod/fmod.hpp"
#include "nlohmann/json.hpp"

#include "nfd.h"
#include "nfd_common.h"
#include "common.h"

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"
#endif

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//c, c++
#include <ctime>
#include <typeindex>
#include <random>
#include <filesystem>
#include <fstream>
#include <hidusage.h>

//thread
#include <thread>
#include <mutex>
#include <atomic>
#include <future>

//stl
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <string>

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_AssetFormat.h"

//util
#include "Utility_Function.h"



#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>


#ifndef USE_IMGUI
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
#endif	//USE_IMGUI