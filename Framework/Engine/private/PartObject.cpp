#include "EnginePCH.h"
#include "PartObject.h"
#include "ContainerObject.h"

PartObject::PartObject()
	:Object()
{
}

PartObject::PartObject(const PartObject& prototype)
	:Object(prototype)
{
}

HRESULT PartObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT PartObject::Initialize(InitDESC* arg)
{
	PART_OBJECT_DESC* desc = static_cast<PART_OBJECT_DESC*>(arg);
	m_pParent = desc->parent;	//여기서 addref하면 상호참조

	/*if use socket*/
	if (desc->parentSocketTransform)
		m_pTransform->SetParent(desc->parentSocketTransform);
	else
		m_pTransform->SetParent(m_pParent->GetComponent<TransformComponent>());

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void PartObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PartObject::Update(_float dt)
{
	__super::Update(dt);
}

void PartObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void PartObject::Free()
{
	__super::Free();
}
