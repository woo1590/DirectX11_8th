#include "EnginePCH.h"
#include "Material.h"

Material::Material()
{
}

Material* Material::Create()
{
	Material* Instance = new Material();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Material::Initialize()
{
	return S_OK;
}

void Material::Free()
{
	__super::Free();
}

void Material::SetParam(const std::string& key, _int value)
{
	m_IntParams[key] = value;
}

void Material::SetParam(const std::string& key, _float value)
{
	m_FloatParams[key] = value;
}

void Material::SetParam(const std::string& key, _float2 value)
{
	m_Float2Params[key] = value;
}

void Material::SetParam(const std::string& key, _float3 value)
{
	m_Float3Params[key] = value;
}

void Material::SetParam(const std::string& key, _float4 value)
{
	m_Float4Params[key] = value;
}
