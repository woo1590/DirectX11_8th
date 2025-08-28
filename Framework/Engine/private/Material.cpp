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
}

void Material::Bind(_uint passIndex)
{
	/*셰이더 바인딩 & 파라미터 세팅*/

	for (const auto& pair : m_TexParams)
		m_pShader->SetValue(pair.first, pair.second);

	m_pShader->Apply(passIndex);
}
