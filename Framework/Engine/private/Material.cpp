#include "EnginePCH.h"
#include "Material.h"
#include "Shader.h"

Material::Material(Shader* pShader)
	:m_pShader(pShader)
{
	m_pShader->AddRef();
}

Material* Material::Create(Shader* pShader)
{
	Material* Instance = new Material(pShader);

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

	Safe_Release(m_pShader);
}

HRESULT Material::BindMaterial(_uint passIndex)
{
	/*Bind SRV*/
	for (const auto& pair : m_TexParams)
	{
		if (FAILED(m_pShader->SetValue(pair.first, pair.second)))
			return E_FAIL;
	}

	/*Bind Matrix*/
	for (const auto& pair : m_Float4x4Params)
	{
		if (FAILED(m_pShader->SetValue(pair.first, pair.second)))
			return E_FAIL;
	}

	return m_pShader->Apply(passIndex);
}
