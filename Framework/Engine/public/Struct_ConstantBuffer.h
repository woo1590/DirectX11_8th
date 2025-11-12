#pragma once

namespace Engine
{
	typedef struct tagConstantBufferPerFrame
	{
		_float4x4 viewMatrix;
		_float4x4 viewMatrixInverse;
		_float4x4 projMatrix;
		_float4x4 projMatrixInverse;

		/*원래는 상수버퍼 따로 만들어서 바인딩 해야하는데 지금은 한번에 다 때려넣기*/
		_float4x4 identityView;
		_float4x4 orthoProjMatrix;

		_float4 camPosition;
		_float farZ;
		_float nearZ;
		_float width{};
		_float height{};
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

	typedef struct tagConstantBufferShadow
	{
		_float4x4 shadowViewMatrix{};
		_float4x4 shadowProjMatrix{};
	}CBShadow;

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
