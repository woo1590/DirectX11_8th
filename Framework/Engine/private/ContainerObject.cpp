#include "EnginePCH.h"
#include "ContainerObject.h"
#include "EngineCore.h"
#include "PartObject.h"

ContainerObject::ContainerObject()
	:Object()
{
}

ContainerObject::ContainerObject(const ContainerObject& prototype)
	:Object(prototype)
{
}

HRESULT ContainerObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ContainerObject::Initialize(InitDESC* arg)
{
	CONTAINER_OBJECT_DESC* desc = static_cast<CONTAINER_OBJECT_DESC*>(arg);
	m_iNumPartObjects = desc->numPartObjects;
	m_PartObjects.resize(m_iNumPartObjects);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void ContainerObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

	for (const auto& part : m_PartObjects)
	{
		if (part)
			part->PriorityUpdate(dt);
	}
}

void ContainerObject::Update(_float dt)
{
	__super::Update(dt);

	for (const auto& part : m_PartObjects)
	{
		if (part)
			part->Update(dt);
	}
}

void ContainerObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	for (const auto& part : m_PartObjects)
	{
		if (part)
			part->LateUpdate(dt);
	}
}

void ContainerObject::Free()
{
	__super::Free();

	for (auto& part : m_PartObjects)
		Safe_Release(part);
	m_PartObjects.clear();
}

HRESULT ContainerObject::AddPartObject(_uint levelID, const _string& prototypeTag, _uint partObjectIndex, InitDESC* arg)
{
	if (partObjectIndex >= m_iNumPartObjects)
	{
		MSG_BOX("Invalild Index");
		return E_FAIL;
	}

	Object* object = EngineCore::GetInstance()->ClonePrototype(levelID, prototypeTag, arg);
	if (!object)
	{
		MSG_BOX("Failed to add PartObject : Not Exist Prototype");
		return E_FAIL;
	}

	PartObject* part = static_cast<PartObject*>(object);
	m_PartObjects[partObjectIndex] = part;

	return S_OK;
}
