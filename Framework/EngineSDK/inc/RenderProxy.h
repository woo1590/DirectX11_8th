#pragma once

NS_BEGIN(Engine)

class VIBuffer;
class Material;
class ModelComponent;
struct RenderProxy
{
	VIBuffer* buffer = nullptr;
	Material* material = nullptr;

	_float4x4 worldMatrix{};
	_float4x4* boneMatrices = nullptr;
	_uint numBones{};

	RenderGroup group{};
	_int frameIndex = 0;
	_float viewZ = 0.f;
};

NS_END