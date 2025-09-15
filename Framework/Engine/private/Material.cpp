#include "EnginePCH.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "EngineCore.h"

Material::Material()
{
}

Material* Material::Create(Shader* pShader)
{
	Material* Instance = new Material();

	if (FAILED(Instance->Initialize(pShader)))
		Safe_Release(Instance);

	return Instance;
}

Material* Material::Create(MTRL_FORMAT mtrlFormat, const _string& modelFilePath)
{
	Material* Instance = new Material();

	if (FAILED(Instance->Initialize(mtrlFormat, modelFilePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Material::Initialize(Shader* pShader)
{
	m_pShader = pShader;
	m_pShader->AddRef();

	return S_OK;
}

HRESULT Material::Initialize(MTRL_FORMAT mtrlFormat, const _string& modelFilePath)
{
	namespace fs = std::filesystem;

	m_pShader = EngineCore::GetInstance()->GetShader(mtrlFormat.shaderTag);
	if (!m_pShader)
		return E_FAIL;
	m_pShader->AddRef();

	fs::path dirPath = fs::path(modelFilePath).parent_path();
	for (_uint i = 0; i < mtrlFormat.numDiffuseTexture; ++i)
	{
		fs::path filePath = dirPath / mtrlFormat.diffuseTextureName[i];
		auto tex = Texture::Create(filePath.string());
		if (!tex)
			return E_FAIL;

		m_TexParams["g_DiffuseTexture"].push_back(tex);
	}

	for (_uint i = 0; i < mtrlFormat.numNormalTexture; ++i)
	{

	}

	for (_uint i = 0; i < mtrlFormat.numSpecularTexture; ++i)
	{

	}

	return S_OK;
}

HRESULT Material::BindMaterial(const _string& passTag, _int frameIndex)
{
	/*Bind Parameter*/

	/*Bind SRV*/
	for (const auto& pair : m_TexParams)
	{
		if (frameIndex >= pair.second.size())
			frameIndex = pair.second.size() - 1;

		if (FAILED(m_pShader->BindTextureValue(pair.first, pair.second[frameIndex])))
			return E_FAIL;
	}

	return m_pShader->Apply(passTag);
}

void Material::SetTexture(const _string& key, Texture* value)
{
	m_TexParams[key].push_back(value);

	value->AddRef();
}

void Material::Free()
{
	__super::Free();

	Safe_Release(m_pShader);

	for (auto& pair : m_TexParams)
	{
		for (auto& tex : pair.second) 
			Safe_Release(tex);
		pair.second.clear();
	}
	m_TexParams.clear();
}