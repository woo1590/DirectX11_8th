#include "EnginePCH.h"
#include "ResourceManager.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Model.h"

ResourceManager::ResourceManager()
{
}

ResourceManager* ResourceManager::Create(_uint numLevel)
{
	ResourceManager* Instance = new ResourceManager();

	if (FAILED(Instance->Initialize(numLevel)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ResourceManager::Initialize(_uint numLevel)
{
	m_iNumLevel = numLevel;

	m_Buffers.resize(numLevel);
	m_Materials.resize(numLevel);
	m_Shaders.resize(numLevel);
	m_Textures.resize(numLevel);
	m_Models.resize(numLevel);

	return S_OK;
}

HRESULT ResourceManager::LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	m_Buffers[levelID].emplace(key, pBuffer);

	return S_OK;
}

HRESULT ResourceManager::LoadTextureFromFile(_uint levelID, const _string& filePath, _uint numTextures)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	auto texture = Texture::Create(filePath, numTextures);
	if (!texture)
		return E_FAIL;

	m_Textures[levelID].emplace(filePath, texture);

	return S_OK;
}

HRESULT ResourceManager::LoadMaterialFromJson(_uint levelID, const _string& filePath, const _string& key)
{
	using json = nlohmann::json;

	json j = json::parse(std::ifstream(filePath.c_str()));
	_string defaultPath = "../bin/resource/";

	if (!j.contains("shader"))
	{
		MSG_BOX("Load Failed : missing shader string");

		return E_FAIL;
	}

	auto shader = GetShader(0, j["shader"].get<_string>());
	if (!shader)
	{
		MSG_BOX("Load Failed : shader not exist");
		return E_FAIL;
	}

	auto material = Material::Create(shader);

	if (j.contains("textures"))
	{
		for (auto& [slot, var] : j["textures"].items())
		{
			_string fullPath = defaultPath + var["path"].get<_string>();
			_uint numTex = var.value("num", 1);

			auto tex = GetTexture(levelID, fullPath);
			if (!tex)
			{
				if (FAILED(LoadTextureFromFile(levelID, fullPath, numTex)))
					return E_FAIL;

				tex = GetTexture(levelID, fullPath);
			}
			material->SetTexture(slot, tex);
		}
	}

	m_Materials[levelID].emplace(key, material);

	return S_OK;
}

HRESULT ResourceManager::LoadShaderFromFile(_uint levelID, const _string& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	auto shader = Shader::Create(filePath, pElement, numElement);
	if (!shader)
		return E_FAIL;

	m_Shaders[levelID].emplace(key, shader);

	return S_OK;
}

HRESULT ResourceManager::LoadModelFromFile(_uint levelID, const _string& filePath, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	auto model = Model::Create(filePath);
	if (!model)
		return E_FAIL;

	m_Models[levelID].emplace(key, model);

	return S_OK;
}

VIBuffer* ResourceManager::GetBuffer(_uint levelID, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return nullptr;

	auto iter = m_Buffers[levelID].find(key);
	
	if (iter == m_Buffers[levelID].end())
		return nullptr;

	return iter->second;
}

Shader* ResourceManager::GetShader(_uint levelID, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return nullptr;

	auto iter = m_Shaders[levelID].find(key);

	if (iter == m_Shaders[levelID].end())
		return nullptr;

	return iter->second;
}

Material* ResourceManager::GetMaterial(_uint levelID, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return nullptr;

	auto iter = m_Materials[levelID].find(key);

	if (iter == m_Materials[levelID].end())
		return nullptr;

	return iter->second;
}

Model* ResourceManager::GetModel(_uint levelID, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return nullptr;

	auto iter = m_Models[levelID].find(key);

	if (iter == m_Models[levelID].end())
		return nullptr;

	return iter->second;
}

Texture* ResourceManager::GetTexture(_uint levelID, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return nullptr;

	auto iter = m_Textures[levelID].find(key);

	if (iter == m_Textures[levelID].end())
		return nullptr;

	return iter->second;
}

void ResourceManager::Clear(_uint levelID)
{
	for (auto& pair : m_Buffers[levelID])
		Safe_Release(pair.second);
	m_Buffers[levelID].clear();

	for (auto& pair : m_Materials[levelID])
		Safe_Release(pair.second);
	m_Materials[levelID].clear();

	for (auto& pair : m_Shaders[levelID])
		Safe_Release(pair.second);
	m_Shaders[levelID].clear();

	for (auto& pair : m_Textures[levelID])
		Safe_Release(pair.second);
	m_Textures[levelID].clear();
}

void ResourceManager::Free()
{
	__super::Free();

	for (auto& buffer : m_Buffers)
	{
		for (auto& pair : buffer)
			Safe_Release(pair.second);
		buffer.clear();
	}
	m_Buffers.clear();

	for (auto& material : m_Materials)
	{
		for (auto& pair : material)
			Safe_Release(pair.second);
		material.clear();
	}
	m_Materials.clear();
	
	for (auto& shader : m_Shaders)
	{
		for (auto& pair : shader)
			Safe_Release(pair.second);
		shader.clear();
	}
	m_Shaders.clear();

	for (auto& texture : m_Textures)
	{
		for (auto& pair : texture)
			Safe_Release(pair.second);
		texture.clear();
	}
	m_Textures.clear();

	for (auto& model : m_Models)
	{
		for (auto& pair : model)
			Safe_Release(pair.second);
		model.clear();
	}
	m_Models.clear();
}
