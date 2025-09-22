#include "EnginePCH.h"
#include "Model.h"
#include "EngineCore.h"
#include "Mesh.h"
#include "Material.h"
#include "Bone.h"
#include "Skeleton.h"
#include "TransformComponent.h"

Model::Model()
{
}

Model* Model::Create(const _string& filePath)
{
	Model* Instance = new Model();

	if (FAILED(Instance->Initialize(filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Model::Initialize(const _string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		MSG_BOX("Failed to load : Model");
		return E_FAIL;
	}

	MODEL_FORMAT modelFormat{};
	file.read(reinterpret_cast<char*>(&modelFormat), sizeof(MODEL_FORMAT));
	m_iNumMeshes = modelFormat.numMeshes;
	m_iNumMaterials = modelFormat.numMaterials;

	m_eType = modelFormat.skinnedMesh ? ModelType::Skinned : ModelType::Static;

	if (FAILED(CreateMeshes(file, modelFormat.preTransformMatrix)))
		return E_FAIL;

	if (FAILED(CreateMaterials(file,filePath)))
		return E_FAIL;

	if (FAILED(CreateSkeleton(file, modelFormat.preTransformMatrix)))
		return E_FAIL;

	return S_OK;
}

void Model::Free()
{
	__super::Free();

	for (auto& mesh : m_Meshes)
		Safe_Release(mesh);
	m_Meshes.clear();

	for (auto& material : m_Materials)
		Safe_Release(material);
	m_Materials.clear();

	Safe_Release(m_pSkeleton);
}

HRESULT Model::CreateMeshes(std::ifstream& file, _float4x4 preTransformMatrix)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		auto mesh = Mesh::Create(m_eType, file, XMLoadFloat4x4(&preTransformMatrix));
		if (!mesh)
		{
			MSG_BOX("Failed to create : Mesh");
			return E_FAIL;
		}
		m_Meshes.push_back(mesh);
	}

	return S_OK;
}

HRESULT Model::CreateMaterials(std::ifstream& file, const _string& filePath)
{
	std::vector<MTRL_FORMAT> mtrlFormats(m_iNumMaterials);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		_uint shaderTagLen = 0;
		file.read(reinterpret_cast<char*>(&shaderTagLen), sizeof(_uint));
		mtrlFormats[i].shaderTag.resize(shaderTagLen);
		file.read(reinterpret_cast<char*>(mtrlFormats[i].shaderTag.data()), shaderTagLen);

		file.read(reinterpret_cast<char*>(&mtrlFormats[i].numDiffuseTexture), sizeof(_uint));
		for (_uint j = 0; j < mtrlFormats[i].numDiffuseTexture; ++j)
		{
			_uint len = 0;
			_string fileName{};
			mtrlFormats[i].diffuseTextureName.resize(mtrlFormats[i].numDiffuseTexture);
			file.read(reinterpret_cast<char*>(&len), sizeof(_uint));
			mtrlFormats[i].diffuseTextureName[j].resize(len);
			file.read(reinterpret_cast<char*>(mtrlFormats[i].diffuseTextureName[j].data()), len);
		}

		file.read(reinterpret_cast<char*>(&mtrlFormats[i].numNormalTexture), sizeof(_uint));
		for (_uint j = 0; j < mtrlFormats[i].numNormalTexture; ++j)
		{
			_uint len = 0;
			_string fileName{};
			mtrlFormats[i].normalTextureName.resize(mtrlFormats[i].numNormalTexture);
			file.read(reinterpret_cast<char*>(&len), sizeof(_uint));
			mtrlFormats[i].normalTextureName[j].resize(len);
			file.read(reinterpret_cast<char*>(mtrlFormats[i].normalTextureName[j].data()), len);
		}
		
		file.read(reinterpret_cast<char*>(&mtrlFormats[i].numSpecularTexture), sizeof(_uint));
		for (_uint j = 0; j < mtrlFormats[i].numSpecularTexture; ++j)
		{
			_uint len = 0;
			_string fileName{};
			mtrlFormats[i].specularTextureName.resize(mtrlFormats[i].numSpecularTexture);
			file.read(reinterpret_cast<char*>(&len), sizeof(_uint));
			mtrlFormats[i].specularTextureName[j].resize(len);
			file.read(reinterpret_cast<char*>(mtrlFormats[i].specularTextureName[j].data()), len);
		}

		mtrlFormats[i].shaderTag;

		auto material = Material::Create(mtrlFormats[i],filePath);
		if (!material)
		{
			MSG_BOX("Failed to create : Material");
			return E_FAIL;
		}

		m_Materials.push_back(material);
	}

	return S_OK;
}

HRESULT Model::CreateSkeleton(std::ifstream& file, _float4x4 preTransformMatrix)
{
	m_pSkeleton = Skeleton::Create(file, preTransformMatrix);
	if (!m_pSkeleton)
	{
		MSG_BOX("Failed to create : Skeleton");
		return E_FAIL;
	}
	
	return S_OK;
}
