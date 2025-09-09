#pragma once

NS_BEGIN(Engine)

class VIBuffer;
class Material;
class ModelComponent;
struct RenderProxy
{
	VIBuffer* buffer = nullptr;
	Material* material = nullptr;
	CBPerObject cbPerObject{};

	RenderGroup group{};
	_int frameIndex = 0;
	_string passTag = {};
	_float viewZ = 0.f;
};

NS_END