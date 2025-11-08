#pragma once
NS_BEGIN(Engine)

namespace math
{
	static constexpr _float PI = XM_PI;
	static constexpr _float TWO_PI = XM_2PI;
	static constexpr _float ELIPSON = 1e-7;

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

		/*Return Radian*/
		return _float3(pitch, yaw, roll);
	}

	inline _float EaseOutQuint(_float t)
	{
		t = std::clamp(t, 0.f, 1.f);

		return 1.f - std::pow(1.f - t, 5.f);
	}

	inline _float EaseOutSline(_float t)
	{
		t = std::clamp(t, 0.f, 1.f);

		return std::sinf(t * PI * 0.5f);
	}

	inline _float SmoothStep(_float t)
	{
		t = std::clamp(t, 0.f, 1.f);

		return t * t * (3.f - 2.f * t);
	}

	inline _float Lerp(_float a, _float b, _float t)
	{
		return a + (b - a) * t;
	}

	inline _float PalabolaCurve(_float t, _float radian)
	{
		return -4.f * radian * t * (1.f - t);
	}

	inline _float PalabolaCurve(_float t)
	{
		return -4.f * t * (1.f - t);
	}

	inline _float DistancePointToLine(_float3 P, _float3 A, _float3 B)
	{
		_vector AB = XMLoadFloat3(&B) - XMLoadFloat3(&A);
		_vector AP = XMLoadFloat3(&P) - XMLoadFloat3(&A);

		_float lengthSqr = XMVectorGetX(XMVector3LengthSq(AB));
		_float t = XMVectorGetX(XMVector3Dot(AB, AP)) / lengthSqr;
		t = std::clamp(t, 0.f, 1.f);

		_vector Q = XMLoadFloat3(&A) + t * AB;

		return XMVectorGetX(XMVector3Length(XMLoadFloat3(&P) - Q));
	}

	inline _bool Elipson(_float a, _float b)
	{
		return (std::abs(a - b) <= ((std::max)(std::abs(a), std::abs(b)) * ELIPSON));
	}

	inline _float4 MakeQuaternionFromDirection(_float3 direction)
	{
		_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector forward = XMVector3Normalize(XMLoadFloat3(&direction));
		_vector right = XMVector3Normalize(XMVector3Cross(worldUp, forward));
		_vector up = XMVector3Normalize(XMVector3Cross(forward, right));

		_float pitch = asin(std::clamp(-XMVectorGetY(forward), -1.f, 1.f));
		_float yaw = atan2(XMVectorGetX(forward), XMVectorGetZ(forward));
		_float roll = 0.f;

		if (fabsf(cosf(pitch)) > 0.0001f) {
			roll = atan2f(XMVectorGetY(right), XMVectorGetY(up));
		}

		_float4 quaternion{};
		XMStoreFloat4(&quaternion, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));

		return quaternion;
	}

	inline _float CalculateTriangleArea(const _float3& p1, const _float3& p2, const _float3& p3)
	{
		_vector v1 = XMVectorSet(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z, 0.0f);
		_vector v2 = XMVectorSet(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z, 0.0f);
		_vector crossProduct = XMVector3Cross(v1, v2);

		_float length = XMVectorGetX(XMVector3Length(crossProduct));

		return length / 2.0f;
	}
};

namespace DXWrap
{
	HRESULT ENGINE_DLL EngineCreateWICTextureFromFile(ID3D11Device* device, _wstring fileName, ID3D11Resource** ppResource, ID3D11ShaderResourceView** ppSRV);

	HRESULT ENGINE_DLL EngineSaveDDSTextureToFile(ID3D11DeviceContext* context, ID3D11Resource* pResource, _wstring outFilePath);
}

namespace map
{
	inline void PrefabToJson(nlohmann::ordered_json& j, const PREFAB& prefab)
	{
		using nlohmann::ordered_json;
		j = ordered_json
		{
			{"PrototypeTag",	prefab.prototypeTag},
			{"ModelTag",		prefab.modelTag},
			{"LayerTag",		prefab.layerTag},
			{"Position",		{{"x",prefab.position.x},{"y",prefab.position.y},{"z",prefab.position.z}}},
			{"Scale",			{{"x",prefab.scale.x},{"y",prefab.scale.y},{"z",prefab.scale.z}}},
			{"Quaternion",		{{"x",prefab.quaternion.x},{"y",prefab.quaternion.y},{"z",prefab.quaternion.z},{"w",prefab.quaternion.w}}}
		};
	}

	inline void PrefabFromJson(const nlohmann::ordered_json& j, PREFAB& prefab)
	{
		prefab.prototypeTag = j.at("PrototypeTag").get<_string>();
		prefab.modelTag = j.at("ModelTag").get<_string>();
		prefab.layerTag = j.at("LayerTag").get<_string>();

		prefab.position.x = j.at("Position").at("x").get<_float>();
		prefab.position.y = j.at("Position").at("y").get<_float>();
		prefab.position.z = j.at("Position").at("z").get<_float>();

		prefab.scale.x = j.at("Scale").at("x").get<_float>();
		prefab.scale.y = j.at("Scale").at("y").get<_float>();
		prefab.scale.z = j.at("Scale").at("z").get<_float>();

		prefab.quaternion.x = j.at("Quaternion").at("x").get<_float>();
		prefab.quaternion.y = j.at("Quaternion").at("y").get<_float>();
		prefab.quaternion.z = j.at("Quaternion").at("z").get<_float>();
		prefab.quaternion.w = j.at("Quaternion").at("w").get<_float>();
	}
}

namespace physics
{
	static constexpr _float RESTITUTION = 1.2f;

	inline void CalculateImpulse(_float3 velocity, _float3 hitNormal, _float3& outVelocity, _float3& outAngularVelocity)
	{
		/*make linear velocity*/
		_vector slideVec{};
		_vector restitutionVec{};
		_float t = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&velocity), XMLoadFloat3(&hitNormal)));

		slideVec = XMLoadFloat3(&velocity) - t * XMLoadFloat3(&hitNormal);
		restitutionVec = XMLoadFloat3(&hitNormal) * restitutionVec;

		XMStoreFloat3(&outVelocity, slideVec + restitutionVec);
	}
}

NS_END