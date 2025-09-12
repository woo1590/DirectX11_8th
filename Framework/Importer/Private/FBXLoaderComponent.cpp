#include "ImporterPCH.h"
#include "FBXLoaderComponent.h"
#include "EngineCore.h"
#include "FBXMesh.h"
#include "TransformComponent.h"
#include "Material.h"

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
	m_pDefaultMtrl = Material::Create(EngineCore::GetInstance()->GetShader(ENUM_CLASS(LevelID::Import), "Shader_VtxMesh"));

	return S_OK;
}

HRESULT FBXLoaderComponent::ExtractRenderProxies(TransformComponent* transform, std::vector<RenderProxy>& proxies)
{
	if (!m_isLoaded)
		return S_OK;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		RenderProxy proxy{};
		CBPerObject cb{};
		cb.worldMatrix = transform->GetWorldMatrix();
		cb.worldMatrixInverse = transform->GetWorldMatrixInverse();

		proxy.buffer = m_Meshes[i];
		proxy.material = m_pDefaultMtrl;
		proxy.cbPerObject = cb;

		proxies.push_back(proxy);
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::LoadModelFromFBX(const _string& filePath)
{
	_uint flag = (aiProcessPreset_TargetRealtime_Fast | aiProcess_PreTransformVertices);
	Assimp::Importer impoter{};

	const aiScene* scene = impoter.ReadFile(filePath.c_str(), flag);
	if (!scene)
	{
		MSG_BOX("Failed to load : wrong file");

		return E_FAIL;
	}

	m_iNumMeshes = scene->mNumMeshes;

	if (FAILED(GenerateMeshes(scene)))
		return E_FAIL;

	m_isLoaded = true;

	return S_OK;
}

HRESULT FBXLoaderComponent::ExportModel(const _string& outFilePath)
{
	std::ofstream out(outFilePath.c_str(), std::ios::binary);
	if (!out.is_open())
	{
		MSG_BOX("Failed to save");
		return E_FAIL;
	}

	MODEL_FORMAT modelFormat{};
	modelFormat.numMeshes = m_iNumMeshes;
	modelFormat.skinnedMesh = 0;
	
	/*Model 포맷 파일 쓰기*/
	out.write(reinterpret_cast<const char*>(&modelFormat), sizeof(MODEL_FORMAT));
		
	/*Mesh 포맷 파일 쓰기*/
	if (FAILED(WriteMeshFormat(out)))
		return E_FAIL;

	MSG_BOX("Save Success");
	return S_OK;
}

void FBXLoaderComponent::Free()
{
	__super::Free();

	for (auto& mesh : m_Meshes)
		Safe_Release(mesh);
	m_Meshes.clear();

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
			if (ImGui::Button("Open.."))
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
				}

				LoadModelFromFBX(openedFile);
				openedFile = "";
			}
		}

		if (m_isLoaded)
		{
			ImGui::SeparatorText("Export");
			{
				if (ImGui::Button("Save"))
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
	}

	ImGui::PopID();
}

HRESULT FBXLoaderComponent::GenerateMeshes(const aiScene* pScene)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		auto mesh = FBXMesh::Create(pScene->mMeshes[i]);
		if (!mesh)
		{
			MSG_BOX("Failed to generate mesh");

			return E_FAIL;
		}

		m_Meshes.push_back(mesh);
	}

	return S_OK;
}

HRESULT FBXLoaderComponent::WriteMeshFormat(std::ofstream& out)
{
	std::vector<MESH_FORMAT> meshFormats(m_iNumMeshes);
	std::vector<std::vector<VTX_FORMAT>> vertexFormats(m_iNumMeshes);
	std::vector<std::vector<_uint>> indices(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		m_Meshes[i]->ExtractMeshFormat(meshFormats[i], vertexFormats[i], indices[i]);
	}


	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		out.write(reinterpret_cast<const char*>(&meshFormats[i]), sizeof(MESH_FORMAT));
		out.write(reinterpret_cast<const char*>(vertexFormats[i].data()), sizeof(VTX_FORMAT) * meshFormats[i].numVertices);
		out.write(reinterpret_cast<const char*>(indices[i].data()), sizeof(_uint) * meshFormats[i].numIndices);
	}

	return S_OK;
}

#endif