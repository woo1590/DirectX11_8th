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

	struct InitDESC {};

	typedef struct tagConstantBufferPerFrame
	{
		_float4x4 viewMatrix;
		_float4x4 viewMatrixInverse;
		_float4x4 projMatrix;
		_float4x4 projMatrixInverse;
		_float4 camPosition;

		_float4 lightColor;
		_float4 lightDirection;
		_float4 lightPosition;
		_float lightRange;
		_float3 pad;

	}CBPerFrame;

	typedef struct tagConstantBufferPerLight
	{
		_float4x4 lightViewMatrix;
		_float4x4 lightProjMatrix;
	}CBPerLight;

	typedef struct tagConstantBufferPerObject
	{
		_float4x4 worldMatrix;

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

	typedef struct tagKeyFrame
	{
		_float3 scale{};
		_float4 rotation{};
		_float3 position{};
		_float trackPosition{};

	}KEYFRAME;

	typedef struct tagAnimationSet
	{
		_uint numAnimations;
		std::vector<class AnimationClip*> aniamtionClips;

	}ANIMATION_SET;

	typedef struct tagAnimationClipContext
	{
		_float trackPosition{};
		std::vector<_uint> keyFrameIndices;
		_bool isLoop = false;
		_bool isFinished = false;

	}ANIMATIONCLIP_CONTEXT;

	typedef struct tagBonePalette
	{
		std::vector<_float4x4> boneMatrices;
	}BONE_PALETTE;
}
#endif // Engine_Struct_h__
