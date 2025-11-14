#include "pch.h"
#include "Editor_EffectContainer.h"
#include "Editor_EffectNode.h"
#include "Editor_SpriteEffect.h"
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
	ContextClear();
	AddNode();
	RemoveNode();

	Import();
	Export();
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

void Editor_EffectContainer::Import()
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();

	static _string openFilePath;
	if (ImGui::Button("Import"))
	{
		nfdchar_t* outPath = nullptr;
		nfdresult_t res = NFD_OpenDialog(nullptr, nullptr, &outPath);

		if (res == NFD_OKAY)
		{
			openFilePath = outPath;
			NFDi_Free(outPath);
		}

		if (!openFilePath.empty())
		{
			std::ifstream file(openFilePath);

			ordered_json container = json::parse(file);

			//m_strEffectName = container.at("name").get<_string>();
			m_iNumPartObjects = container.at("num_node").get<_uint>();
			m_fDuration = container.at("duration").get<_float>();

			_float3 position = m_pTransform->GetPosition();
			_float3 scale = m_pTransform->GetScale();
			_float4 quaternion = m_pTransform->GetQuaternion();

			position.x = container.at("position").at("x").get<_float>();
			position.y = container.at("position").at("y").get<_float>();
			position.z = container.at("position").at("z").get<_float>();

			scale.x = container.at("scale").at("x").get<_float>();
			scale.y = container.at("scale").at("y").get<_float>();
			scale.z = container.at("scale").at("z").get<_float>();

			quaternion.x = container.at("quaternion").at("x").get<_float>();
			quaternion.y = container.at("quaternion").at("y").get<_float>();
			quaternion.z = container.at("quaternion").at("z").get<_float>();
			quaternion.w = container.at("quaternion").at("w").get<_float>();

			for (auto& node : container["nodes"])
			{
				_string type = node.at("type").get<_string>();
				if ("sprite" == type)
				{
					Editor_EffectNode::EFFECT_NODE_DESC desc{};
					desc.effectType = EffectType::Sprite;
					desc.parent = this;
					desc.context = &m_Context;

					Object* effectNode = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpriteEffect", &desc);
					m_PartObjects.push_back(static_cast<PartObject*>(effectNode));

					static_cast<Editor_SpriteEffect*>(effectNode)->Import(node);
				}
			}

			m_pTransform->SetPosition(position);
			m_pTransform->SetScale(scale);
			m_pTransform->SetQuaternion(quaternion);

		}
	}
}

void Editor_EffectContainer::Export()
{
	using namespace nlohmann;
	namespace fs = std::filesystem;

	static _string outFilePath;
	if (ImGui::Button("Export"))
	{
		nfdchar_t* outPath = nullptr;
		nfdresult_t res = NFD_SaveDialog(nullptr, nullptr, &outPath);

		if (res == NFD_OKAY)
		{
			outFilePath = outPath;
			NFDi_Free(outPath);
		}

		if (!outFilePath.empty())
		{
			ordered_json container = ordered_json::object();

			//container["name"] = m_strEffectName;
			container["num_node"] = m_iNumPartObjects;
			container["duration"] = m_fDuration;

			_float3 position = m_pTransform->GetPosition();
			_float3 scale = m_pTransform->GetScale();
			_float4 quaternion = m_pTransform->GetQuaternion();
			container["position"] = { {"x",position.x},{"y",position.y},{"z",position.z} };
			container["scale"] = { {"x",scale.x},{"y",scale.y},{"z",scale.z} };
			container["quaternion"] = { {"x",quaternion.x},{"y",quaternion.y},{"z",quaternion.z},{"w",quaternion.w} };

			container["nodes"] = json::array();

			for (auto& node : m_PartObjects)
			{
				static_cast<Editor_EffectNode*>(node)->Export(container["nodes"]);
			}

			std::ofstream out(outFilePath);
			if (!out.is_open())
			{
				MSG_BOX("Failed to save");
				return;
			}

			out << container.dump(2);
		}
	}
}

void Editor_EffectContainer::LoadTextureFromDirectory(const _string& dirPath)
{
	namespace fs = std::filesystem;

	for (auto& entry : fs::recursive_directory_iterator(dirPath))
	{
		if (!entry.is_regular_file())
			continue;
		
		auto tex = Texture::Create(entry.path().string());
		if (!tex)
		{
			_string path = entry.path().string();
			MSG_BOX("Failed to load : %s", path.c_str());
			continue;
		}

		_string fileName = entry.path().stem().string();
		m_Textures.emplace(fileName, tex);
	}

	m_Context.pAllTextures = &m_Textures;
}

void Editor_EffectContainer::DisplayTextures()
{
	ImGui::Begin("Texture");
	float avail = ImGui::GetWindowSize().x;
	int   cols = (std::max)(1, (int)((avail + 8.f) / (128.f + 8.f)));
	float cellW = (avail - 8.f * (cols - 1)) / cols;
	float cellH = cellW;

	_uint idx = 0;
	_uint count = 0;
	for (auto& pair : m_Textures)
	{
		if (idx % cols != 0) ImGui::SameLine();
		ImGui::PushID(idx);

		ImGui::BeginGroup();
		ImVec2 p0 = ImGui::GetCursorScreenPos();
		ImVec2 size(cellW, cellH);
		_bool isPressed = ImGui::InvisibleButton(("imgbtn" + std::to_string(idx)).c_str(), size);

		_bool isLeftClick = ImGui::IsItemClicked(ImGuiMouseButton_Left);
		_bool isRightClick = ImGui::IsItemClicked(ImGuiMouseButton_Right);

		_float iw = 1.f;
		_float ih = 1.f;
		_float scale = (std::min)(size.x / iw, size.y / ih);
		ImVec2 drawSize(iw * scale, ih * scale);
		ImVec2 offset((size.x - drawSize.x) * 0.5f, (size.y - drawSize.y) * 0.5f);

		ImTextureID texID = (ImTextureID)pair.second->GetSRV();
		ImGui::GetWindowDrawList()->AddImage(texID,
			ImVec2(p0.x + offset.x, p0.y + offset.y),
			ImVec2(p0.x + offset.x + drawSize.x, p0.y + offset.y + drawSize.y));


		for (const auto& select : m_SelectIndices)
		{
			_bool isHovered = ImGui::IsItemHovered();
			_bool isSelected = (select == static_cast<_int>(idx));
			if (isHovered || isSelected)
			{
				ImU32 col = isSelected ? IM_COL32(80, 170, 255, 255) : IM_COL32(255, 255, 255, 180);
			
				ImVec2 rMin = ImGui::GetItemRectMin();
				ImVec2 rMax = ImGui::GetItemRectMax();
				ImGui::GetWindowDrawList()->AddRect(rMin, rMax, col, 6.0f, 0, 2.0f);
			}
		}

		if (isLeftClick)
		{
			auto iter = std::find(m_SelectIndices.begin(), m_SelectIndices.end(), idx);
			if (iter == m_SelectIndices.end())
			{
				m_SelectIndices.push_back(idx);
				m_Context.textureTags.push_back(pair.first);
				m_Context.textures.push_back(pair.second);
			}
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
	auto engine = EngineCore::GetInstance();

	if (ImGui::Button("Add Node : Sprite"))
	{
		Editor_EffectNode::EFFECT_NODE_DESC desc{};
		desc.effectType = EffectType::Sprite;
		desc.parent = this;
		desc.context = &m_Context;

		Object* effectNode = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpriteEffect", &desc);
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

void Editor_EffectContainer::ContextClear()
{
	if (ImGui::Button("Context Clear"))
	{
		m_SelectIndices.clear();
		m_Context.textureTags.clear();
		m_Context.textures.clear();
	}
}
