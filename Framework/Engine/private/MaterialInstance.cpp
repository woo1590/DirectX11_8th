#include "EnginePCH.h"
#include "MaterialInstance.h"
#include "Shader.h"

MaterialInstance::MaterialInstance()
{
}

MaterialInstance* MaterialInstance::Create()
{
	MaterialInstance* Instance = new MaterialInstance();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MaterialInstance::Initialize()
{
	return S_OK;
}

HRESULT MaterialInstance::BindMaterialInstance(Shader* shader)
{
	/*Int params*/
	for (const auto& pair : m_IntParams)
	{
		if (FAILED(shader->BindRawValue(pair.first, &pair.second, sizeof(_int))))
		{
			MSG_BOX("Failed to bind int : MaterialInstance");
			return E_FAIL;
		}
	}
	/*Float params*/
	for (const auto& pair : m_FloatParams)
	{
		if (FAILED(shader->BindRawValue(pair.first, &pair.second, sizeof(_float))))
		{
			MSG_BOX("Failed to bind float : MaterialInstance");
			return E_FAIL;
		}
	}
	/*Float2 params*/
	for (const auto& pair : m_Float2Params)
	{
		if (FAILED(shader->BindRawValue(pair.first, &pair.second, sizeof(_float2))))
		{
			MSG_BOX("Failed to bind float2 : MaterialInstance");
			return E_FAIL;
		}
	}
	/*Float3 params*/
	for (const auto& pair : m_Float3Params)
	{
		if (FAILED(shader->BindRawValue(pair.first, &pair.second, sizeof(_float3))))
		{
			MSG_BOX("Failed to bind float3: MaterialInstance");
			return E_FAIL;
		}
	}
	/*Float4 params*/
	for (const auto& pair : m_Float4Params)
	{
		if (FAILED(shader->BindRawValue(pair.first, &pair.second, sizeof(_float4))))
		{
			MSG_BOX("Failed to bind float4 : MaterialInstance");
			return E_FAIL;
		}
	}
	/*Float4x4 params*/
	for (const auto& pair : m_Float4x4Params)
	{
		if (FAILED(shader->BindMatrix(pair.first, &pair.second)))
		{
			MSG_BOX("Failed to bind float4x4 : MaterialInstance");
			return E_FAIL;
		}
	}


	return S_OK;
}

void MaterialInstance::SetInt(_string name, _int param)
{
	m_IntParams[name] = param;
}

void MaterialInstance::SetFloat(_string name, _float param)
{
	m_FloatParams[name] = param;
}

void MaterialInstance::SetFloat2(_string name, _float2 param)
{
	m_Float2Params[name] = param;
}

void MaterialInstance::SetFloat3(_string name, _float3 param)
{
	m_Float3Params[name] = param;
}

void MaterialInstance::SetFloat4(_string name, _float4 param)
{
	m_Float4Params[name] = param;
}

void MaterialInstance::SetFloat4x4(_string name, _float4x4 param)
{
	m_Float4x4Params[name] = param;
}

void MaterialInstance::Free()
{
	__super::Free();
}
