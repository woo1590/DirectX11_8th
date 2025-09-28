#pragma once

namespace Engine
{
	typedef struct tagModelFormat
	{
		_uint numMeshes;
		_uint numMaterials;
		_uint skinnedMesh;	//0 static , 1 skinned
		_float4x4 preTransformMatrix{};

	}MODEL_FORMAT;

	typedef struct tagMeshForamt
	{
		_char name[MAX_PATH];
		_uint materialIndex;

		_uint numVertices;
		_uint vertexStride;

		_uint numIndices;
		_uint indexStride;

		_uint numBones;
		_uint boneIndices[MAX_BONES];
		//_float4x4 offsetMatrices[MAX_BONES]; 오프셋 행렬 따로 넘기기

	}MESH_FORMAT;

	typedef struct tagVertexFormat
	{
		_float3 position;
		_float3 normal;
		_float2 texCoord;
		_float3 tangent;

		/*--only skinned mesh--*/
		_uint4 blendIndex;
		_float4 blendWeight;

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
	
	typedef struct tagSkeletonFormat
	{
		_uint numBones{};
	}SKELETON_FORMAT;
	
	typedef struct tagBoneFormat
	{
		_char boneTag[MAX_PATH];
		_int parentIndex;
		_float4x4 transformationMatrix{};
		_float4x4 combinedTransformationMatrix{};

	}BONE_FORMAT;

	typedef struct tagAnimationClipFormat
	{
		_char animationTag[MAX_PATH];
		_uint numChannels{};
		_float tickPerSecond{};
		_float duration{};

	}ANIMATIONCLIP_FORMAT;

	typedef struct tagChannelFormat
	{
		_char channelTag[MAX_PATH];
		_uint boneIndex{};
		_uint numKeyFrames{};

	}CHANNEL_FORMAT;
}