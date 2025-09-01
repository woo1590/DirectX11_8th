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

	typedef struct tagConstantBufferPerFrame
	{
		_float4x4 viewMatrix;
		_float4x4 viewMatrixInverse;
		_float4x4 projMatrix;
		_float4x4 projMatrixInverse;
		_float4 camPosition;
	}CBPerFrame;

	typedef struct tagConstantBufferPerLight
	{
		_float4x4 lightViewMatrix;
		_float4x4 lightProjMatrix;
	}CBPerLight;

	typedef struct tagConstantBufferPerObject
	{
		_float4x4 worldMatrix;
		_float4x4 worldMatrixInverse;
	}CBPerObject;

	typedef struct tagVertexPosition
	{
		_float3 position;

		static constexpr _uint numElement = 1;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement]=
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	}VTXPOS;

	typedef struct tagVertexTexCoord
	{
		_float3 position;
		_float2 texCoord;

		static constexpr _uint numElement = 2;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	
	}VTXTEX;

	typedef struct VertexCube
	{
		_float3 position;
		_float3 texCoord;

		static constexpr _uint numElement = 2;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	}VTXCUBE;

	typedef struct tagVertexPositionNormalTexCoord
	{
		_float3 position;
		_float3 normal;
		_float2 texCoord;

		static constexpr _uint numElement = 3;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXNOR;
}
#endif // Engine_Struct_h__
