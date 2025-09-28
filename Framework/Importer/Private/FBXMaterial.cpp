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

	m_strName = aiMat->GetName().data;
	m_strModelFilePath = modelFilePath;
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

		fs::path name = filePath.C_Str();

		if (name.stem().string() == "default")
			return S_OK;

		fs::path fullPath = dirPath / fs::path(filePath.C_Str()).filename();
		auto tex = Texture::Create(fullPath.string());
		if (!tex)
		{
			MSG_BOX("Failed to load : Texture(FBXMaterial)");
			return E_FAIL;
		}

		m_TexParams["g_DiffuseTexture"].push_back(tex);
		m_TexNames["g_DiffuseTexture"].push_back(fullPath.filename().replace_extension(".dds").string());
	}

	count = aiMat->GetTextureCount(aiTextureType_NORMALS);
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

		m_TexParams["g_NormalTexture"].push_back(tex);
		m_TexNames["g_NormalTexture"].push_back(fullPath.filename().replace_extension(".dds").string());
	}

	count = aiMat->GetTextureCount(aiTextureType_SPECULAR);
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

		m_TexParams["g_SpecularTexture"].push_back(tex);
		m_TexNames["g_SpecularTexture"].push_back(fullPath.filename().replace_extension(".dds").string());
	}

	return S_OK;
}

HRESULT FBXMaterial::Export(MTRL_FORMAT& mtrlFormat)
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

HRESULT FBXMaterial::ConvertToDDS(const _string& outFilePath)
{
	auto device = EngineCore::GetInstance()->GetDevice();
	auto context = EngineCore::GetInstance()->GetDeviceContext();

	namespace fs = std::filesystem;
	fs::path outDirPath = fs::path(outFilePath).parent_path();
	fs::path modelDirPath = fs::path(m_strModelFilePath).parent_path();

	for (const auto& pair : m_TexNames)
	{
		for (const auto& fileName : pair.second)
		{
			ID3D11Resource* tex = nullptr;
			ID3D11ShaderResourceView* srv = nullptr;
			fs::path loadFilePath = modelDirPath / fs::path(fileName).replace_extension(".png");
			fs::path outFilePath = outDirPath / fs::path(fileName);
			if (FAILED(DXWrap::EngineCreateWICTextureFromFile(device, loadFilePath.wstring(), &tex, &srv)))
				return E_FAIL;

			context->GenerateMips(srv);

			if (FAILED(DXWrap::EngineSaveDDSTextureToFile(context, tex, outFilePath.wstring())))
				return E_FAIL;
		}
	}

	return S_OK;
}

void FBXMaterial::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI

void FBXMaterial::RenderInspector(_uint id)
{
	ImGui::PushID(this);
	_string name = std::to_string(id) + " : " + m_strName;
	if (ImGui::CollapsingHeader(name.c_str(),
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::SeparatorText("Diffuse");
		DrawTextureGrid("g_DiffuseTexture");
		ImGui::SeparatorText("Normal");
		DrawTextureGrid("g_NormalTexture");
		ImGui::SeparatorText("Specular");
		DrawTextureGrid("g_SpecularTexture");
	}

	ImGui::PopID();
}
void FBXMaterial::DrawTextureGrid(const _string& tag)
{
	static _float thumb = 96.f;
	static _float pad = 8.f;

	auto iter = m_TexNames.find(tag);
	if (iter != m_TexNames.end())
	{
		const float avail = ImGui::GetContentRegionAvail().x;
		const int columns = (std::max)(1, (int)((avail + pad) / (thumb + pad)));
		if (ImGui::BeginTable(tag.c_str(), columns)) {
			for (int i = 0; i < (int)m_TexParams[tag].size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::PushID(i);

				ID3D11ShaderResourceView* srv = m_TexParams[tag][i]->GetSRV();
				if (srv) {
					ImGui::Image((ImTextureID)srv, ImVec2(thumb, thumb));
				}
				else {
					ImGui::Dummy(ImVec2(thumb, thumb));
				}

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}
	ImGui::PushID(tag.c_str());
	if (ImGui::Button("Add Texture"))
	{
		_string openedFile{};
		nfdchar_t* outPath = nullptr;
		nfdresult_t res = NFD_OpenDialog(nullptr, nullptr, &outPath);

		if (res == NFD_OKAY)
		{
			openedFile = outPath;
			NFDi_Free(outPath);
		}

		if (!openedFile.empty())
		{
			if (FAILED(AddTexture(tag, openedFile)))
				MSG_BOX("Faild to load!");
		}
	}
	ImGui::PopID();
}
#endif

HRESULT FBXMaterial::AddTexture(const _string& slotTag, const _string& filePath)
{
	namespace fs = std::filesystem;

	auto tex = Texture::Create(filePath);
	if (!tex)
	{
		MSG_BOX("Failed to load : Texture -> FBXMaterial::AddTexture()");
		return E_FAIL;
	}

	_string fileName = fs::path(filePath).filename().replace_extension(".dds").string();
	m_TexParams[slotTag].push_back(tex);
	m_TexNames[slotTag].push_back(fileName);

	return S_OK;
}