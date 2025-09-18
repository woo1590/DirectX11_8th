#include "EnginePCH.h"
#include "ResourceManager.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Model.h"
#include "AnimationClip.h"

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
	m_Textures.resize(numLevel);
	m_Models.resize(numLevel);
	m_AnimationSets.resize(numLevel);

	return S_OK;
}

HRESULT ResourceManager::LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	m_Buffers[levelID].emplace(key, pBuffer);

	return S_OK;
}

HRESULT ResourceManager::LoadTextureFromFile(_uint levelID, const _string& filePath)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	auto texture = Texture::Create(filePath);
	if (!texture)
		return E_FAIL;

	m_Textures[levelID].emplace(filePath, texture);

	return S_OK;
}

HRESULT ResourceManager::LoadMaterialFromJson(_uint levelID, const _string& filePath, const _string& key)
{
	using json = nlohmann::json;
	namespace fs = std::filesystem;

	json j = json::parse(std::ifstream(filePath.c_str()));
	_string defaultPath = "../bin/resource/";

	if (!j.contains("shader"))
	{
		MSG_BOX("Load Failed : missing shader string");

		return E_FAIL;
	}

	auto shader = GetShader(j["shader"].get<_string>());
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
			fs::path fullPath = defaultPath + var["path"].get<_string>();
			_uint numTex = var.value("num", 1);

			if (numTex > 1)
			{
				fs::path dirPath = fullPath.parent_path();

				for (_uint i = 0; i < numTex; ++i)
				{
					fs::path fileName = fullPath.stem().string() + std::to_string(i) + fullPath.extension().string();
					fs::path fullPathNum = dirPath / fileName;

					auto tex = GetTexture(levelID, fullPathNum.string());
					if (!tex)
					{
						if (FAILED(LoadTextureFromFile(levelID, fullPathNum.string())))
							return E_FAIL;

						tex = GetTexture(levelID, fullPathNum.string());
					}
					material->SetTexture(slot, tex);
				}
			}
			else
			{
				auto tex = GetTexture(levelID, fullPath.string());
				if (!tex)
				{
					if (FAILED(LoadTextureFromFile(levelID, fullPath.string())))
						return E_FAIL;

					tex = GetTexture(levelID, fullPath.string());
				}
				material->SetTexture(slot, tex);
			}
		}
	}

	m_Materials[levelID].emplace(key, material);

	return S_OK;
}

HRESULT ResourceManager::LoadShaderFromFile(const _string& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
	auto shader = Shader::Create(filePath, pElement, numElement);
	if (!shader)
		return E_FAIL;

	m_Shaders.emplace(key, shader);

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

HRESULT ResourceManager::LoadAnimationSetFromFile(_uint levelID, const _string& filePath, const _string& key)
{
	if (levelID >= m_iNumLevel)
		return E_FAIL;

	std::ifstream file(filePath,std::ifstream::binary);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load : AnimationSet");
		return E_FAIL;
	}

	ANIMATION_SET animationSet{};
	file.read(reinterpret_cast<char*>(&animationSet.numAnimations), sizeof(_uint));

	animationSet.aniamtionClips.resize(animationSet.numAnimations);
	for (_uint i = 0; i < animationSet.numAnimations; ++i)
	{
		auto animationClip = AnimationClip::Create(file);
		if (!animationClip)
		{
			MSG_BOX("Failed to create : Animation Clip");
			return E_FAIL;
		}

		animationSet.aniamtionClips[i] = animationClip;
	}

	m_AnimationSets[levelID].emplace(key, animationSet);

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

Shader* ResourceManager::GetShader(const _string& key)
{
	auto iter = m_Shaders.find(key);

	if (iter == m_Shaders.end())
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
	
	for (auto& pair : m_Shaders)
		Safe_Release(pair.second);
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

	for (auto& animation : m_AnimationSets)
	{
		for (auto& pair : animation)
			for (auto& clip : pair.second.aniamtionClips)
				Safe_Release(clip);
		animation.clear();
	}
	m_AnimationSets.clear();
		
}
