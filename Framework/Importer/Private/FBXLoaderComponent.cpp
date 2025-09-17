#include "ImporterPCH.h"
#include "FBXLoaderComponent.h"
#include "EngineCore.h"
#include "FBXMesh.h"
#include "Object.h"
#include "TransformComponent.h"
#include "FBXMaterial.h"
#include "FBXBone.h"

FBXLoaderComponent::FBXLoaderComponent(Object* owner)
	:Component(owner)
{
}

FBXLoaderComponent::FBXLoaderComponent(const FBXLoaderComponent& prototype)
	:Component(prototype)
{
}

FBXLoaderComponent* FBXLoaderComponent::Create(Object* owner)
{
	FBXLoaderComponent* Instance = new FBXLoaderComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXLoaderComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT FBXLoaderComponent::Initialize(InitDESC* arg)
{
	m_pDefaultMtrl = Material::Create(EngineCore::GetInstance()->GetShader("Shader_VtxMesh"));
	m_strShaderTag = "Shader_VtxSkinnedMesh";

	return S_OK;
}

void FBXLoaderComponent::Update(_float dt)
{
	__super::Update(dt);

	for (const auto& bone : m_Bones)
		bone->UpdateCombinedTransformMatrix(m_Bones);
}

HRESULT FBXLoaderComponent::ExtractRenderProxies(TransformComponent* transform, std::vector<RenderProxy>& proxies)
{
	if (!m_isLoaded)
		return S_OK;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		RenderProxy proxy{};
		proxy.worldMatrix = transform->GetWorldMatrix();

		proxy.buffer = m_Meshes[i];
		proxy.material = m_Materials[m_Meshes[i]->GetMaterialIndex()];

		if (m_iNumBones)
			m_Meshes[i]->ExtractBoneMatrices(proxy, m_Bones);

		proxies.push_back(proxy);
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::ImportModel(const _string& filePath)
{
	XMStoreFloat4x4(&m_PreTransformMatrix, XMMatrixIdentity());

	_uint flag = aiProcessPreset_TargetRealtime_Fast;

	if (m_eType == ModelType::Static)
		flag |= aiProcess_PreTransformVertices;

	Assimp::Importer impoter{};

	const aiScene* scene = impoter.ReadFile(filePath.c_str(), flag);
	if (!scene)
	{
		MSG_BOX("Failed to load : wrong file");

		return E_FAIL;
	}

	m_iNumMeshes = scene->mNumMeshes;

	if (FAILED(GenerateBones(scene->mRootNode, -1)))
		return E_FAIL;
	m_iNumBones = m_Bones.size();

	if (FAILED(GenerateMaterials(scene, filePath)))
		return E_FAIL;

	if (FAILED(GenerateMeshes(scene)))
		return E_FAIL;

	if (FAILED(GenerateAnimation(scene)))
		return E_FAIL;

	m_isLoaded = true;

	return S_OK;
}

HRESULT FBXLoaderComponent::ExportModel(const _string& outFilePath)
{
	namespace fs = std::filesystem;

	std::ofstream out(outFilePath.c_str(), std::ios::binary);
	if (!out.is_open())
	{
		MSG_BOX("Failed to save");
		return E_FAIL;
	}

	m_PreTransformMatrix = m_pOwner->GetComponent<TransformComponent>()->GetWorldMatrix();

	MODEL_FORMAT modelFormat{};
	modelFormat.numMeshes = m_iNumMeshes;
	modelFormat.numMaterials = m_iNumMaterials;
	modelFormat.numBones = m_iNumBones;
	modelFormat.skinnedMesh = (m_eType == ModelType::Static ? 0 : 1);
	modelFormat.preTransformMatrix = m_PreTransformMatrix;

	/*Output Directory*/
	
	/*Model 포맷 파일 쓰기*/
	out.write(reinterpret_cast<const char*>(&modelFormat), sizeof(MODEL_FORMAT));
		
	/*Mesh 포맷 파일 쓰기*/
	if (FAILED(WriteMeshFormat(out)))
		return E_FAIL;

	/*Material 포맷 파일 쓰기*/
	if (FAILED(WriteMaterialFormat(out)))
		return E_FAIL;

	/*Bone 포맷 파일 쓰기*/
	if (FAILED(WriteBoneFormat(out)))
		return E_FAIL;

	for (const auto& mtrl : m_Materials)
		mtrl->ConvertToDDS(outFilePath);

	MSG_BOX("Save Success");
	return S_OK;
}

_int FBXLoaderComponent::GetBoneIndexByName(const _string& boneName)
{
	_int index = 0;
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](FBXBone* bone)
		{
			if (boneName == bone->GetBoneTag())
				return true;

			++index;
			return false;
		});

	if (iter == m_Bones.begin())
		return -1;
	else
		return index;
}

void FBXLoaderComponent::Free()
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

	Safe_Release(m_pDefaultMtrl);
}

#ifdef USE_IMGUI

void FBXLoaderComponent::RenderInspector()
{
	namespace fs = std::filesystem;

	static _string defaultPath = "../Bin/fbx/";
	static _string openedFile{};
	static _string savedFileName{};
	static _string currDir{};

	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("FBXLoader",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::SeparatorText("Import");
		{
			if (ImGui::Button("Change Import Model Type"))
				m_eType = m_eType == ModelType::Static ? ModelType::Skinned : ModelType::Static;

			ImGui::Text("Model Type : %s", m_eType == ModelType::Static ? "Static" : "Skinned");

			if (ImGui::Button("Import.."))
			{
				nfdchar_t* outPath = nullptr;
				nfdresult_t res = NFD_OpenDialog(nullptr, nullptr, &outPath);

				if (res == NFD_OKAY)
				{
					openedFile = outPath;
					NFDi_Free(outPath);
				}
			}

			if (!openedFile.empty())
			{
				ImGui::Text("Opened: %s", openedFile.c_str());
				if (!m_Meshes.empty())
				{
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

				ImportModel(openedFile);
				openedFile = "";
			}
		}

		if (m_isLoaded)
		{
			ImGui::SeparatorText("Materials");
			for (const auto& mtrl : m_Materials)
				mtrl->RenderInspector();

			if (ImGui::Button("Export.."))
			{
				nfdchar_t* outPath = nullptr;
				nfdresult_t res = NFD_SaveDialog(nullptr, nullptr, &outPath);

				if (res == NFD_OKAY)
				{
					savedFileName = outPath;
					NFDi_Free(outPath);

					ExportModel(savedFileName);
				}
			}
		}
	}

	ImGui::PopID();
}

#endif

HRESULT FBXLoaderComponent::GenerateMeshes(const aiScene* pScene)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		auto mesh = FBXMesh::Create(pScene->mMeshes[i], m_eType, this);
		if (!mesh)
		{
			MSG_BOX("Failed to generate mesh");

			return E_FAIL;
		}

		m_Meshes.push_back(mesh);
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::GenerateMaterials(const aiScene* pScene, const _string& modelFilePath)
{
	m_iNumMaterials = pScene->mNumMaterials;
	auto shader = EngineCore::GetInstance()->GetShader(m_strShaderTag);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* aiMat = pScene->mMaterials[i];

		auto material = FBXMaterial::Create(shader, m_strShaderTag, aiMat, modelFilePath);
		if (!material)
			return E_FAIL;

		m_Materials.push_back(material);
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::GenerateBones(const aiNode* pNode, _int parentIndex)
{
	auto bone = FBXBone::Create(pNode, parentIndex);
	if (!bone)
		return E_FAIL;

	m_Bones.push_back(bone);
	
	_int index = m_Bones.size() - 1;
	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		if (FAILED(GenerateBones(pNode->mChildren[i], index)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::GenerateAnimation(const aiScene* pScene)
{
	_uint numAnim = pScene->mNumAnimations;
	for (_uint i = 0; i < numAnim; ++i)
	{
		aiAnimation* ani = pScene->mAnimations[i];
		ani->mName;
		ani->mMeshChannels;
		int a = 1;
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::WriteMeshFormat(std::ofstream& out)
{
	std::vector<MESH_FORMAT> meshFormats(m_iNumMeshes);
	std::vector<std::vector<VTX_FORMAT>> vertexFormats(m_iNumMeshes);
	std::vector<std::vector<_uint>> indices(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
		m_Meshes[i]->ExportMeshFormat(meshFormats[i], vertexFormats[i], indices[i]);


	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		out.write(reinterpret_cast<const char*>(&meshFormats[i]), sizeof(MESH_FORMAT));
		out.write(reinterpret_cast<const char*>(vertexFormats[i].data()), sizeof(VTX_FORMAT) * meshFormats[i].numVertices);
		out.write(reinterpret_cast<const char*>(indices[i].data()), sizeof(_uint) * meshFormats[i].numIndices);
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::WriteMaterialFormat(std::ofstream& out)
{
	std::vector<MTRL_FORMAT> mtrlFormats(m_iNumMaterials);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
		m_Materials[i]->ExportMaterialFormat(mtrlFormats[i]);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		_uint shaderTagLen = mtrlFormats[i].shaderTag.length();
		out.write(reinterpret_cast<const char*>(&shaderTagLen), sizeof(_uint));
		out.write(reinterpret_cast<const char*>(mtrlFormats[i].shaderTag.c_str()), shaderTagLen);

		out.write(reinterpret_cast<const char*>(&mtrlFormats[i].numDiffuseTexture), sizeof(_uint));
		for (_uint j = 0; j < mtrlFormats[i].numDiffuseTexture; ++j)
		{
			_uint len = mtrlFormats[i].diffuseTextureName[j].length();
			out.write(reinterpret_cast<const char*>(&len), sizeof(_uint));
			out.write(reinterpret_cast<const char*>(mtrlFormats[i].diffuseTextureName[j].data()), len);
		}

		out.write(reinterpret_cast<const char*>(&mtrlFormats[i].numNormalTexture), sizeof(_uint));
		for (_uint j = 0; j < mtrlFormats[i].numNormalTexture; ++j)
		{
			_uint len = mtrlFormats[i].normalTextureName[j].length();
			out.write(reinterpret_cast<const char*>(&len), sizeof(_uint));
			out.write(reinterpret_cast<const char*>(mtrlFormats[i].normalTextureName[j].data()), len);
		}

		out.write(reinterpret_cast<const char*>(&mtrlFormats[i].numSpecularTexture), sizeof(_uint));
		for (_uint j = 0; j < mtrlFormats[i].numSpecularTexture; ++j)
		{
			_uint len = mtrlFormats[i].specularTextureName[j].length();
			out.write(reinterpret_cast<const char*>(&len), sizeof(_uint));
			out.write(reinterpret_cast<const char*>(mtrlFormats[i].specularTextureName[j].data()), len);
		}
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::WriteBoneFormat(std::ofstream& out)
{
	std::vector<BONE_FORMAT> boneFormats(m_iNumBones);
	for (_uint i = 0; i < m_iNumBones; ++i)
		m_Bones[i]->ExportBoneFormat(boneFormats[i]);

	out.write(reinterpret_cast<const char*>(boneFormats.data()), sizeof(BONE_FORMAT) * m_iNumBones);

	return S_OK;
}
