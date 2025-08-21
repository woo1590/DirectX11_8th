#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	struct EngineDESC
	{
		HWND hWnd;
		WinMode winMode;
		_uint winSizeX, winSizeY;
		_uint levelCnt;
	};
}
#endif // Engine_Struct_h__
