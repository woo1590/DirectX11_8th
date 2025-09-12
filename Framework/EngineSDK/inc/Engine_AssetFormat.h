#pragma once

namespace Engine
{
	typedef struct tagModelFormat
	{
		_uint numMeshes;
		_uint skinnedMesh;	//0 static , 1 skinned

	}MODEL_FORMAT;

	typedef struct tagMeshForamt
	{
		_uint numVertices;
		_uint vertexStride;
		
		_uint numIndices;
		_uint indexStride;

	}MESH_FORMAT;

	typedef struct tagVertexFormat
	{
		_float3 position;
		_float3 normal;
		_float2 texCoord;
		_float3 tangent;

	}VTX_FORMAT;

	typedef struct tagMaterialFormat
	{
		
	}MTRL_FORMAT;

	typedef struct tagBoneFormat
	{

	}BONE_FORMAT;
}