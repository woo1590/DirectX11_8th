#include "EnginePCH.h"
#include "ResourceManager.h"
#include "VIBuffer.h"
#include "Shader.h"
#include "Material.h"

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

	return S_OK;
}

HRESULT ResourceManager::LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	m_Buffers[levelID].emplace(key, pBuffer);

	return S_OK;
}

HRESULT ResourceManager::LoadShader(_uint levelID, const _wstring& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	auto shader = Shader::Create(filePath, pElement, numElement);
	if (!shader)
		return E_FAIL;

	m_Shaders[levelID].emplace(key, shader);

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

}
