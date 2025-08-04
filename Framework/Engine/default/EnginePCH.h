#ifndef EnginePCH_H
#define EnginePCH_H

//d3d
#include <d3d11.h>
#include <DirectXMath.h>

//c, c++
#include <ctime>
#include <typeindex>
#include <random>

//stl
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <string>

//library
#include "../../External/fmod/inc/fmod.hpp"
#include "../../External/nlohmann/json.hpp"

//define
#include "Engine_Defines.h"

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace Engine;

#endif // !EnginePCH_H
