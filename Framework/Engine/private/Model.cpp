#include "EnginePCH.h"
#include "Model.h"
#include "EngineCore.h"
#include "Mesh.h"
#include "Material.h"
#include "Bone.h"
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
	m_iNumBones = modelFormat.numBones;
	m_PreTransformMatrix = modelFormat.preTransformMatrix;

	m_eType = modelFormat.skinnedMesh ? ModelType::Skinned : ModelType::Static;

	if (FAILED(CreateMeshes(file)))
		return E_FAIL;

	if (FAILED(CreateMaterials(file,filePath)))
		return E_FAIL;

	if (FAILED(CreateBones(file)))
		return E_FAIL;

	return S_OK;
}

void Model::Update(_float dt)
{
	for (const auto& bone : m_Bones)
		bone->UpdateCombinedTransformMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
}

HRESULT Model::ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies, Material* overrideMaterial)
{
	_float4x4 worldMatrix = transform->GetWorldMatrix();

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		RenderProxy proxy{};

		proxy.buffer = m_Meshes[i];
		proxy.material = overrideMaterial ? overrideMaterial : m_Materials[m_Meshes[i]->GetMaterialIndex()];
		proxy.worldMatrix = worldMatrix;

		if (m_iNumBones)
			m_Meshes[i]->ExtractBoneMatrices(proxy, m_Bones);

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

	for (auto& bone : m_Bones)
		Safe_Release(bone);
	m_Bones.clear();
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

		auto mesh = Mesh::Create(m_eType, meshFormats[i], vertexFormats[i], indexFormats[i], XMLoadFloat4x4(&m_PreTransformMatrix));
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

HRESULT Model::CreateBones(std::ifstream& file)
{
	std::vector<BONE_FORMAT> boneFormats(m_iNumBones);

	file.read(reinterpret_cast<char*>(boneFormats.data()), sizeof(BONE_FORMAT) * m_iNumBones);
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		auto bone = Bone::Create(boneFormats[i]);
		if (!bone)
			return E_FAIL;

		m_Bones.push_back(bone);
	}

	return S_OK;
}
