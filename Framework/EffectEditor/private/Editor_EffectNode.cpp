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

HRESULT Editor_EffectNode::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EffectNode";

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

void Editor_EffectNode::Free()
{
	__super::Free();
}
