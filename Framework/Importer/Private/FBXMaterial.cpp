#include "ImporterPCH.h"
#include "FBXMaterial.h"
#include "Shader.h"
#include "Texture.h"
#include "EngineCore.h"

FBXMaterial::FBXMaterial()
	:Material()
{
}

FBXMaterial* FBXMaterial::Create(Shader* pShader,const _string& shaderTag, aiMaterial* aiMat, const _string& modelFilePath)
{
	FBXMaterial* Instance = new FBXMaterial();

	if (FAILED(Instance->Initialize(pShader,shaderTag ,aiMat, modelFilePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXMaterial::Initialize(Shader* pShader, const _string& shaderTag, aiMaterial* aiMat, const _string& modelFilePath)
{
	namespace fs = std::filesystem;

	m_strShaderTag = shaderTag;

	m_pShader = pShader;
	m_pShader->AddRef();

	auto engine = EngineCore::GetInstance();

	fs::path modelPath = modelFilePath;
	fs::path dirPath = modelPath.parent_path();

	_uint count = 0;

	count = aiMat->GetTextureCount(aiTextureType_DIFFUSE);
	for (_uint i = 0; i < count; ++i)
	{
		aiString filePath;
		aiMat->GetTexture(aiTextureType_DIFFUSE, i, &filePath);

		fs::path fullPath = dirPath / fs::path(filePath.C_Str()).filename();
		auto tex = Texture::Create(fullPath.string());
		if (!tex)
		{
			MSG_BOX("Failed to load : Texture(FBXMaterial)");
			return E_FAIL;
		}

		m_TexParams["g_DiffuseTexture"].push_back(tex);
		m_TexNames["g_DiffuseTexture"].push_back(fullPath.filename().string());
	}

	return S_OK;
}

HRESULT FBXMaterial::ExportMaterialFormat(MTRL_FORMAT& mtrlFormat)
{
	mtrlFormat.shaderTag = m_strShaderTag;

	/*Export Diffuse*/	
	mtrlFormat.numDiffuseTexture = m_TexNames["g_DiffuseTexture"].size();
	for (_uint i = 0; i < mtrlFormat.numDiffuseTexture; ++i)
		mtrlFormat.diffuseTextureName.push_back(m_TexNames["g_DiffuseTexture"][i]);

	/*Export Normal*/	
	mtrlFormat.numNormalTexture = m_TexNames["g_NormalTexture"].size();
	for (_uint i = 0; i < mtrlFormat.numNormalTexture; ++i)
		mtrlFormat.normalTextureName.push_back(m_TexNames["g_NormalTexture"][i]);

	/*Export Specular*/
	mtrlFormat.numSpecularTexture = m_TexNames["g_SpecularTexture"].size();
	for (_uint i = 0; i < mtrlFormat.numSpecularTexture; ++i)
		mtrlFormat.specularTextureName.push_back(m_TexNames["g_SpecularTexture"][i]);

	return S_OK;
}

void FBXMaterial::Free()
{
	__super::Free();
}
