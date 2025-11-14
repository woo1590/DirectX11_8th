#pragma once

NS_BEGIN(Engine)

class VIBuffer;
class Material;
class MaterialInstance;
class ColliderComponent;
struct RenderProxy
{
	VIBuffer* buffer = nullptr;
	Material* material = nullptr;
	MaterialInstance* materialInstance = nullptr;

	_float4x4 worldMatrix{};
	_int frameIndex = 0;

	/*Bone palette*/
	_float4x4* boneMatrices = nullptr;
	_uint numBones{};

	/*For blend object*/
	_float viewZ = 0.f;

	/*For Particle system*/

	/*For Debug*/
	ColliderComponent* collider = nullptr;

	RenderGroup group{};
};

NS_END