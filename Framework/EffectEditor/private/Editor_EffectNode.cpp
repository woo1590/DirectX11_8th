#include "pch.h"
#include "Editor_EffectNode.h"
#include "MaterialInstance.h"
#include "SpriteComponent.h"
#include "Material.h"
#include "Texture.h"
#include "Editor_EffectContainer.h"

_uint Editor_EffectNode::m_iMaterialIndex = 0;

Editor_EffectNode::Editor_EffectNode()
	:PartObject()
{
}

Editor_EffectNode::Editor_EffectNode(const Editor_EffectNode& prototype)
	:PartObject(prototype)
{
}

Editor_EffectNode* Editor_EffectNode::Create()
{
	Editor_EffectNode* Instance = new Editor_EffectNode();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Editor_EffectNode::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EffectNode";
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT Editor_EffectNode::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	EFFECT_NODE_DESC* desc = static_cast<EFFECT_NODE_DESC*>(arg);
	m_eEffectType = desc->effectType;
	m_pContext = desc->context;

	return S_OK;
}

void Editor_EffectNode::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Editor_EffectNode::Update(_float dt)
{
	__super::Update(dt);
}

void Editor_EffectNode::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Editor_EffectNode::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	switch (m_eEffectType)
	{
	case EffectEditor::EffectType::Mesh:
	{

	}break;
	case EffectEditor::EffectType::Sprite:
	{
		auto sprite = GetComponent<SpriteComponent>();
		if (sprite->IsFinished())
			return S_OK;

		__super::ExtractRenderProxies(proxies);

	}break;
	case EffectEditor::EffectType::Particle:
	{

	}break;
	default:
		break;
	}

	return S_OK;
}

void Editor_EffectNode::RenderInspector()
{
	ImGui::PushID(this);
	
	switch (m_eEffectType)
	{
	case EffectEditor::EffectType::Mesh:
		break;
	case EffectEditor::EffectType::Sprite:
	{
		SpriteEffectSetUp();
	}break;
	case EffectEditor::EffectType::Particle:
		break;
	case EffectEditor::EffectType::Count:
		SetType();
		break;
	default:
		break;
	}

	__super::RenderInspector();

	ImGui::PopID();
}

void Editor_EffectNode::Start()
{
	switch (m_eEffectType)
	{
	case EffectEditor::EffectType::Mesh:
		break;
	case EffectEditor::EffectType::Sprite:
	{
		auto sprite = GetComponent<SpriteComponent>();
		sprite->SetFrameIndex(0);
	}break;
	case EffectEditor::EffectType::Particle:
		break;
	default:
		break;
	}
}

Object* Editor_EffectNode::Clone(InitDESC* arg)
{
	Editor_EffectNode* Instance = new Editor_EffectNode(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Editor_EffectNode::Free()
{
	__super::Free();

	Safe_Release(m_pSpriteMaterial);
}

void Editor_EffectNode::SetType()
{
	auto engine = EngineCore::GetInstance();

	if (ImGui::Button("Mesh"))
		m_eEffectType = EffectType::Mesh;
	if (ImGui::Button("Sprite"))
	{
		m_eEffectType = EffectType::Sprite;
		_string materialKey = "EditorMaterial" + std::to_string(m_iMaterialIndex);
		auto shader = engine->GetShader("Shader_VtxPoint");
		engine->LoadMaterial(ENUM_CLASS(LevelID::Static), materialKey, Material::Create(shader));
		
		auto sprite = AddComponent<SpriteComponent>();
		sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
		sprite->SetMaterial(ENUM_CLASS(LevelID::Static), materialKey);

		SpriteComponent::SPRITE_DESC desc{};
		sprite->Initialize(&desc);

		m_pSpriteMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), materialKey);
		m_pSpriteMaterial->AddRef();

		++m_iMaterialIndex;
	}
	if (ImGui::Button("Particle"))
		m_eEffectType = EffectType::Particle;
}

void Editor_EffectNode::SpriteEffectSetUp()
{
	_bool isDirty = false;

	auto sprite = GetComponent<SpriteComponent>();

	isDirty |= ImGui::DragFloat("Speed : ", &m_fSpeed);
	isDirty |= ImGui::DragInt("MaxFrameIndex : ", &m_iMaxFrameIndex);
	isDirty |= ImGui::Checkbox("Is Repeat : ", &m_IsRepeat);
	isDirty |= ImGui::Checkbox("Is Animated : ", &m_IsAnimated);

	if (isDirty)
	{
		SpriteComponent::SPRITE_DESC desc{};
		desc.fSpeed = m_fSpeed;
		desc.iMaxFrameIndex = m_iMaxFrameIndex;
		desc.isRepeat = m_IsRepeat;
		desc.isAnimated = m_IsAnimated;

		sprite->Initialize(&desc);
	}

	if (ImGui::Button("Add Sprite Texture"))
		AddCurrTexture();
	if (ImGui::Button("Replace Material"))
		ReplaceMaterial();
}

void Editor_EffectNode::AddCurrTexture()
{
	if(m_pContext->texture)
		m_pSpriteMaterial->SetTexture("g_DiffuseTexture", m_pContext->texture);
}

void Editor_EffectNode::ReplaceMaterial()
{
	Safe_Release(m_pSpriteMaterial);

	auto engine = EngineCore::GetInstance();

	_string materialKey = "EditorMaterial" + std::to_string(m_iMaterialIndex);
	auto shader = engine->GetShader("Shader_VtxPoint");
	engine->LoadMaterial(ENUM_CLASS(LevelID::Static), materialKey, Material::Create(shader));

	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), materialKey);

	m_pSpriteMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), materialKey);
	m_pSpriteMaterial->AddRef();

	++m_iMaterialIndex;
}
