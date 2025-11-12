#include "pch.h"
#include "Editor_EffectContainer.h"
#include "Editor_EffectNode.h"
#include "Texture.h"

Editor_EffectContainer::Editor_EffectContainer()
	:ContainerObject()
{
}

Editor_EffectContainer::Editor_EffectContainer(const Editor_EffectContainer& prototype)
	:ContainerObject(prototype)
{
}

Editor_EffectContainer* Editor_EffectContainer::Create()
{
	Editor_EffectContainer* Instance = new Editor_EffectContainer();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Editor_EffectContainer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EffectContainer";

	return S_OK;
}

HRESULT Editor_EffectContainer::Initialize(InitDESC* arg)
{
	EFFECT_CONTAINER_DESC desc{};
	desc.numPartObjects = 0;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	LoadTextureFromDirectory("../bin/resource/");

	return S_OK;
}

void Editor_EffectContainer::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Editor_EffectContainer::Update(_float dt)
{
	__super::Update(dt);

}

void Editor_EffectContainer::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

#ifdef USE_IMGUI
void Editor_EffectContainer::RenderInspector()
{
	DisplayTextures();
	AddNode();
	RemoveNode();
	Play();

	__super::RenderInspector();
}
#endif

Object* Editor_EffectContainer::Clone(InitDESC* arg)
{
	Editor_EffectContainer* Instance = new Editor_EffectContainer(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Editor_EffectContainer::Free()
{
	__super::Free();

	for (auto& pair : m_Textures)
		Safe_Release(pair.second);
	m_Textures.clear();
}

void Editor_EffectContainer::Play()
{
	if (ImGui::Button("Play"))
	{
		for (auto& node : m_PartObjects)
			static_cast<Editor_EffectNode*>(node)->Start();
	}
}

void Editor_EffectContainer::Import(const _string& filePath)
{
}

void Editor_EffectContainer::Export(const _string& outFilePath)
{

}

void Editor_EffectContainer::LoadTextureFromDirectory(const _string& dirPath)
{
	namespace fs = std::filesystem;

	for (auto& entry : fs::directory_iterator(dirPath))
	{
		if (!entry.is_regular_file())
			continue;
		
		auto tex = Texture::Create(entry.path().string());
		if (!tex)
		{
			MSG_BOX("Failed to load : %s", entry.path().string().c_str());
			continue;
		}

		_string fileName = entry.path().stem().string();
		m_Textures.emplace(fileName, tex);
	}
}

void Editor_EffectContainer::DisplayTextures()
{
	ImGui::Begin("Texture");
	float avail = ImGui::GetWindowSize().x;
	int   cols = (std::max)(1, (int)((avail + 8.f) / (128.f + 8.f)));
	float cellW = (avail - 8.f * (cols - 1)) / cols;
	float cellH = cellW; // 정사각 타일

	_uint idx = 0;
	_uint count = 0;
	static _int select = -1;
	for (auto& pair : m_Textures)
	{
		if (idx % cols != 0) ImGui::SameLine();
		ImGui::PushID(idx);

		ImGui::BeginGroup();
		ImVec2 p0 = ImGui::GetCursorScreenPos();
		ImVec2 size(cellW, cellH);
		_bool isPressed = ImGui::InvisibleButton(("imgbtn" + std::to_string(idx)).c_str(), size);

		_float iw = 1.f;
		_float ih = 1.f;
		_float scale = (std::min)(size.x / iw, size.y / ih);
		ImVec2 drawSize(iw * scale, ih * scale);
		ImVec2 offset((size.x - drawSize.x) * 0.5f, (size.y - drawSize.y) * 0.5f);

		// SRV를 ImTextureID로
		ImTextureID texID = (ImTextureID)pair.second->GetSRV();
		ImGui::GetWindowDrawList()->AddImage(texID,
			ImVec2(p0.x + offset.x, p0.y + offset.y),
			ImVec2(p0.x + offset.x + drawSize.x, p0.y + offset.y + drawSize.y));

		_bool isHovered = ImGui::IsItemHovered();
		_bool isSelected = (select == static_cast<_int>(idx));
		if (isHovered || isSelected)
		{
			ImU32 col = isSelected ? IM_COL32(80, 170, 255, 255) : IM_COL32(255, 255, 255, 180);
			
			ImVec2 rMin = ImGui::GetItemRectMin();
			ImVec2 rMax = ImGui::GetItemRectMax();
			ImGui::GetWindowDrawList()->AddRect(rMin, rMax, col, 6.0f, 0, 2.0f);
		}

		if (isPressed)
		{
			select = idx;
			m_Context.textureTag = pair.first;
			m_Context.texture = pair.second;
		}

		ImGui::TextWrapped("%s", pair.first.c_str());
		ImGui::EndGroup();
		ImGui::PopID();
		++idx;
	}
	ImGui::End();
}

void Editor_EffectContainer::AddNode()
{
	if (ImGui::Button("Add Node"))
	{
		auto engine = EngineCore::GetInstance();

		Editor_EffectNode::EFFECT_NODE_DESC desc{};
		desc.parent = this;
		desc.context = &m_Context;

		Object* effectNode = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectNode", &desc);
		m_PartObjects.push_back(static_cast<PartObject*>(effectNode));

		m_iNumPartObjects = m_PartObjects.size();
	}
}

void Editor_EffectContainer::RemoveNode()
{
	if (m_PartObjects.empty())
		return;

	if (ImGui::Button("Remove Last Node"))
	{
		Safe_Release(m_PartObjects.back());
		m_PartObjects.pop_back();

		m_iNumPartObjects = m_PartObjects.size();
	}
}
