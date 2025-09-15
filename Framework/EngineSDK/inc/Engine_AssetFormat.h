#pragma once

namespace Engine
{
	typedef struct tagModelFormat
	{
		_uint numMeshes;
		_uint numMaterials;
		_uint skinnedMesh;	//0 static , 1 skinned
		_string dirPath{};

	}MODEL_FORMAT;

	typedef struct tagMeshForamt
	{
		_uint materialIndex;

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
		_string shaderTag{};

		/*Diffuse Texture*/
		_uint numDiffuseTexture{};
		std::vector<_string> diffuseTextureName;

		/*Normal Texture*/	
		_uint numNormalTexture{};
		std::vector<_string> normalTextureName;

		/*Specular Texture*/	
		_uint numSpecularTexture{};
		std::vector<_string> specularTextureName;
		
	}MTRL_FORMAT;

	typedef struct tagBoneFormat
	{

	}BONE_FORMAT;
}