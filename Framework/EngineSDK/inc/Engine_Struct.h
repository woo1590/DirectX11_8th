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

	struct InitDESC{};

	typedef struct ENGINE_DLL tagVertexPosition
	{
		XMFLOAT3 position;

		static constexpr _uint numElement = 1;
		static const D3D11_INPUT_ELEMENT_DESC elements[numElement];

	}VTXPOS;

	typedef struct ENGINE_DLL VertexTexCoord
	{
		XMFLOAT3 position;
		XMFLOAT2 texCoord;

		static constexpr _uint numElement = 2;
		static const D3D11_INPUT_ELEMENT_DESC elements[numElement];
	
	}VTXTEX;

	typedef struct VertexCube
	{
		XMFLOAT3 position;
		XMFLOAT3 texCoord;

		static constexpr _uint numElement = 2;
		static const D3D11_INPUT_ELEMENT_DESC elements[numElement];
	}VTXCUBE;

}
#endif // Engine_Struct_h__
