#include "EnginePCH.h"
#include "Model.h"
#include "EngineCore.h"
#include "Mesh.h"
#include "Material.h"
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

	if (FAILED(CreateMeshes(file)))
		return E_FAIL;

	if (FAILED(CreateMaterials(file,filePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT Model::ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies, Material* overrideMaterial)
{
	_float4x4 worldMatrix = transform->GetWorldMatrix();
	_float4x4 worldMatrixInverse = transform->GetWorldMatrixInverse();

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		RenderProxy proxy{};

		CBPerObject cb{};
		cb.worldMatrix = worldMatrix;
		cb.worldMatrixInverse = worldMatrixInverse;
		
		proxy.buffer = m_Meshes[i];
		proxy.material = overrideMaterial ? overrideMaterial : m_Materials[m_Meshes[i]->GetMaterialIndex()];
		proxy.cbPerObject = cb;

		proxies.push_back(proxy);
	}

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
}

HRESULT Model::CreateMeshes(std::ifstream& file)
{
	std::vector<MESH_FORMAT> meshFormats(m_iNumMeshes);
	std::vector<std::vector<VTX_FORMAT>> vertexFormats(m_iNumMeshes);
	std::vector<std::vector<_uint>> indexFormats(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		file.read(reinterpret_cast<char*>(&meshFormats[i]), sizeof(MESH_FORMAT));
		vertexFormats[i].resize(meshFormats[i].numVertices);
		indexFormats[i].resize(meshFormats[i].numIndices);

		file.read(reinterpret_cast<char*>(vertexFormats[i].data()), sizeof(VTX_FORMAT) * meshFormats[i].numVertices);
		file.read(reinterpret_cast<char*>(indexFormats[i].data()), sizeof(_uint) * meshFormats[i].numIndices);

		auto mesh = Mesh::Create(meshFormats[i], vertexFormats[i], indexFormats[i]);
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
		file.read(reinterpret_cast<char*>(mtrlFormats[i].shaderTag.data()), shaderTagLen);

		file.read(reinterpret_cast<char*>(&mtrlFormats[i].numDiffuseTexture), sizeof(_uint));

		for (_uint j = 0; j < mtrlFormats[i].numDiffuseTexture; ++j)
		{
			_uint len = 0;
			_string fileName{};
			mtrlFormats[i].diffuseTextureName.resize(mtrlFormats[i].numDiffuseTexture);
			file.read(reinterpret_cast<char*>(&len), sizeof(_uint));
			file.read(reinterpret_cast<char*>(mtrlFormats[i].diffuseTextureName[j].data()), len);
		}

		for (_uint j = 0; j < mtrlFormats[i].numNormalTexture; ++j)
		{
			_uint len = 0;
			_string fileName{};
			file.read(reinterpret_cast<char*>(&len), sizeof(_uint));
			file.read(reinterpret_cast<char*>(mtrlFormats[i].normalTextureName[j].data()), len);
		}

		for (_uint j = 0; j < mtrlFormats[i].numSpecularTexture; ++j)
		{
			_uint len = 0;
			_string fileName{};
			file.read(reinterpret_cast<char*>(&len), sizeof(_uint));
			file.read(reinterpret_cast<char*>(mtrlFormats[i].specularTextureName[j].data()), len);
		}

		auto material = Material::Create(mtrlFormats[i],filePath);
		if (!material)
			return E_FAIL;

		m_Materials.push_back(material);
	}

	return S_OK;
}
