#pragma once

namespace Engine
{
	typedef struct tagBVHNode
	{
		BoundingBox nodeAABB{};
		_int left = -1;
		_int right = -1;
		_uint firstIndex = 0;
		_uint count = 0;
	}BVH_NODE;

	typedef struct tagTriangleDesc
	{
		BoundingBox triAABB{};
		_float3 position[3];
		_uint index[3];
	}TRIANGLE_DESC;
}