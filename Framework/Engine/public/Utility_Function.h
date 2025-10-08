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

		/*Return Radian*/
		return _float3(pitch, yaw, roll);
	}

	inline _float EaseOutQuint(_float t)
	{
		t = std::clamp(t, 0.f, 1.f);

		return 1.f - std::pow(1.f - t, 5.f);
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

NS_END