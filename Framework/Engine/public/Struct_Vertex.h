#pragma once

namespace Engine
{
	typedef struct tagVertexPosition
	{
		_float3 position;

		static constexpr _uint numElement = 1;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
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
	}VTXNORTEX;

	typedef struct tagVertexMesh
	{
		_float3 position;
		_float3 normal;
		_float2 texCoord;
		_float3 tangent;

		static constexpr _uint numElement = 4;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXMESH;

	typedef struct tagVertexSkinnedMesh
	{
		_float3 position;
		_float3 normal;
		_float2 texCoord;
		_float3 tangent;

		_uint4 blendIndex{};
		_float4 blendWeight{};

		static constexpr _uint numElement = 6;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXSKINNEDMESH;

	typedef struct tagVertexCell
	{
		_float3 position;

		static constexpr _uint numElement = 1;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXCELL;

	typedef struct tagVertexCollider
	{
		_float3 position;

		static constexpr _uint numElement = 1;
		static constexpr D3D11_INPUT_ELEMENT_DESC elements[numElement] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXCOLLIDER;
}