#pragma once

NS_BEGIN(Engine)

struct LightProxy
{
	LightType type{};
	_float4 lightColor{};
	
	//directional light
	_float4 lightDirection{};
	
	//point, spot light
	_float4 lightPosition{};
	_float lightRange{};
};

NS_END