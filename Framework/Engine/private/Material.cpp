#include "EnginePCH.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "EngineCore.h"

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

HRESULT Material::BindMaterial(const _string& passTag, _int frameIndex)
{
	/*Bind SRV*/
	for (const auto& pair : m_TexParams)
	{
		if (FAILED(m_pShader->BindTextureValue(pair.first, pair.second, frameIndex)))
			return E_FAIL;
	}

	return m_pShader->Apply(passTag);
}

void Material::SetTexture(const _string& key, Texture* value)
{
	m_TexParams[key] = value;

	value->AddRef();
}

void Material::Free()
{
	__super::Free();

	Safe_Release(m_pShader);

	for (auto& pair : m_TexParams)
		Safe_Release(pair.second);
	m_TexParams.clear();
}