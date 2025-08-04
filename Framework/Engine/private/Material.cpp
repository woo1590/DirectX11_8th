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
	intParams[key] = value;
}

void Material::SetParam(const std::string& key, _float value)
{
	floatParams[key] = value;
}
