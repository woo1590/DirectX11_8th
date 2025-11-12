#include "pch.h"
#include "EffectNode.h"

EffectNode::EffectNode()
	:PartObject()
{
}

EffectNode::EffectNode(const EffectNode& prototype)
	:PartObject(prototype)
{
}

EffectNode* EffectNode::Create()
{
	EffectNode* Instance = new EffectNode();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectNode::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT EffectNode::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void EffectNode::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EffectNode::Update(_float dt)
{
	__super::Update(dt);
}

void EffectNode::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EffectNode::Clone(InitDESC* arg)
{
	EffectNode* Instance = new EffectNode(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectNode::Free()
{
	__super::Free();
}
