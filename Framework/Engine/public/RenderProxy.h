#pragma once

NS_BEGIN(Engine)

class VIBuffer;
class Material;
class MaterialInstance;
class ColliderComponent;
class CustomFont;
struct RenderProxy
{
	VIBuffer* buffer = nullptr;
	Material* material = nullptr;
	MaterialInstance* materialInstance = nullptr;
	CustomFont* font = nullptr;

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

	/*Custom Render Func*/
	std::function<HRESULT()> renderFunc;

	RenderGroup group{};
};

NS_END