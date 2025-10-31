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

	ComputeBoundingBox();

	return S_OK;
}

RAY_HIT_DATA Model::RayCastLocal(RAY ray, PickingType type)
{
	RAY_HIT_DATA hitData{};

	_float distance = FLT_MAX;
	_float3 normal{};
	if (m_BoundingBox.Intersects(XMLoadFloat3(&ray.origin), XMVector3Normalize(XMLoadFloat3(&ray.direction)), distance))
	{
		if (type == PickingType::BoundingBox)
		{
			hitData.isHit = true;
			hitData.localDistance = distance;
		}
		else
		{
			_float minDistance = FLT_MAX;
			if (m_iNumMeshes > m_Meshes.size())
				int a = 1;

			for (_uint i = 0; i < m_iNumMeshes; ++i)
			{
				RAY_HIT_DATA meshHitData{};
				meshHitData = m_Meshes[i]->RayCast(ray, type);

				if (meshHitData.isHit && meshHitData.localDistance < minDistance)
				{
					hitData.isHit = true;
					minDistance = meshHitData.localDistance;
					normal = meshHitData.normal;
				}
			}

			hitData.localDistance = minDistance;
			hitData.normal = normal;
		}
	}

	return hitData;
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

void Model::ComputeBoundingBox()
{
	_float3 min{ FLT_MAX,FLT_MAX,FLT_MAX };
	_float3 max{ -FLT_MAX,-FLT_MAX,-FLT_MAX };

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		BOUNDING_BOX_DATA meshBoudingBox = m_Meshes[i]->GetMeshBoundingBoxData();
		_float3 meshMin = meshBoudingBox.AABBMin;
		_float3 meshMax = meshBoudingBox.AABBMax;

		if (meshMin.x < min.x) min.x = meshMin.x;
		if (meshMin.y < min.y) min.y = meshMin.y;
		if (meshMin.z < min.z) min.z = meshMin.z;

		if (meshMax.x > max.x) max.x = meshMax.x;
		if (meshMax.y > max.y) max.y = meshMax.y;
		if (meshMax.z > max.z) max.z = meshMax.z;
	}

	m_AABBMin = min;
	m_AABBMax = max;

	m_BoundingBox.CreateFromPoints(m_BoundingBox, XMLoadFloat3(&m_AABBMin), XMLoadFloat3(&m_AABBMax));
}
