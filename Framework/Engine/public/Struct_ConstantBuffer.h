#pragma once

namespace Engine
{
	typedef struct tagConstantBufferPerFrame
	{
		_float4x4 viewMatrix;
		_float4x4 viewMatrixInverse;
		_float4x4 projMatrix;
		_float4x4 projMatrixInverse;
		_float4 camPosition;
		_float farZ;
		_float nearZ;
		_float2 pad;
	}CBPerFrame;

	typedef struct tagConstantBufferPerLight
	{
		_float4 lightColor;
		_float4 lightDirection;
		_float4 lightPosition;
		_float lightRange;
		_float3 pad;
	}CBPerLight;

	typedef struct tagConstantBufferPerObject
	{
		_float4x4 worldMatrix;
	}CBPerObject;


	typedef struct tagDebugConstantBufferPerFrame
	{
		_float4x4 viewMatrix{};
		_float4x4 projMatrix{};

	}DEBUG_CBPerFrame;

	typedef struct tagDebugConstantBufferPerDraw
	{
		_float4x4 worldMatrix{};

	}DEBUG_CBPerDraw;
}
