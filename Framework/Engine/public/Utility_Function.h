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

	inline _float3 ToEuler(_fvector quat)
	{
		_float4x4 rotMat;
		XMStoreFloat4x4(&rotMat, XMMatrixRotationQuaternion(quat));

		_float pitch, yaw, roll;

		_float s = -rotMat._23;
		if (s < -1.f)
			s = -1.f;
		else if (s > 1.f)
			s = 1.f;

		pitch = asinf(s);
		if (fabsf(cosf(pitch) > 0.0001f))
		{
			yaw = atan2(rotMat._13, rotMat._33);
			roll = atan2(rotMat._21, rotMat._22);
		}
		else
		{
			yaw = atan2(-rotMat._31,rotMat._11);
			roll = 0.f;
		}

		pitch = XMConvertToDegrees(pitch);
		yaw = XMConvertToDegrees(yaw);
		roll = XMConvertToDegrees(roll);

		return _float3(pitch, yaw, roll);
	}
};

namespace DXWrap
{
	HRESULT ENGINE_DLL EngineCreateWICTextureFromFile(ID3D11Device* device, _wstring fileName, ID3D11Resource** ppResource, ID3D11ShaderResourceView** ppSRV);

	HRESULT ENGINE_DLL EngineSaveDDSTextureToFile(ID3D11DeviceContext* context, ID3D11Resource* pResource, _wstring outFilePath);
}

NS_END