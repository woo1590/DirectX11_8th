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

	inline void TransposeGuizmoMatrix(_float* colMatrix, _float4x4 rowMatrix)
	{
		XMMATRIX m = XMLoadFloat4x4(&rowMatrix);

		XMStoreFloat4x4(reinterpret_cast<_float4x4*>(colMatrix), m);
	}
};

NS_END