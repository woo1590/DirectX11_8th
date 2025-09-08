#pragma once
NS_BEGIN(Engine)

namespace math
{
	static constexpr _float PI = XM_PI;
	static constexpr _float TWO_PI = XM_2PI;

	inline _float ToRadian(_float degree)
	{
		return XMConvertToRadians(degree);
	}

	inline _float ToDegree(_float radian)
	{
		return XMConvertToDegrees(radian);
	}

	inline _float ToFmodVector(_float3 vector)
	{
		return 1.f;
	}

};

NS_END