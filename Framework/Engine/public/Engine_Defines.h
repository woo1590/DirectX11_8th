#pragma once

//d3d
#include <d3d11.h>
#include <DirectXMath.h>

//c, c++
#include <ctime>
#include <typeindex>
#include <random>

//thread
#include <thread>
#include <mutex>
#include <atomic>

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

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
