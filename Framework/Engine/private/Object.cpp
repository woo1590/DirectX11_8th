#include "EnginePCH.h"
#include "Object.h"
#include "TransformComponent.h"

Object::Object()
{
}

Object::Object(const Object& prototype)
{
	for (const auto& pair : prototype.m_ComponentMap)
	{
		auto comp = pair.second->Clone();
		comp->SetOwner(this);

		m_Components.push_back(comp);
		m_ComponentMap[pair.first] = comp;
		comp->AddRef();
	}

	m_pTransform = GetComponent<TransformComponent>();
	m_pTransform->AddRef();
}

HRESULT Object::Initialize_Prototype()
{
	m_Components.clear();
	m_ComponentMap.clear();

	m_pTransform = AddComponent<TransformComponent>();
	if (!m_pTransform)
		return E_FAIL;
	
	m_pTransform->AddRef();

	return S_OK;
}

HRESULT Object::Initialize(InitDESC* arg)
{
	if (arg)
	{
		ObjectDESC* objectDesc = static_cast<ObjectDESC*>(arg);
		m_strInstanceTag = objectDesc->instanceTag;

		if (FAILED(m_pTransform->Initialize(arg)))
			return E_FAIL;
	}

	return S_OK;
}

void Object::PriorityUpdate(_float dt)
{
	for (const auto& comp : m_Components)
		comp->PriorityUpdate(dt);
}

void Object::Update(_float dt)
{
	for (const auto& comp : m_Components)
		comp->Update(dt);
}

void Object::LateUpdate(_float dt)
{
	for (const auto& comp : m_Components)
		comp->LateUpdate(dt);
}

void Object::Free()
{
	for (auto& comp : m_Components)
		Safe_Release(comp);

	for (auto& [key, comp] : m_ComponentMap)
		Safe_Release(comp);

	Safe_Release(m_pTransform);

	m_Components.clear();
	m_ComponentMap.clear();
}

#ifdef USE_IMGUI

void Object::RenderInspector()
{
	for (const auto& comp : m_Components)
		comp->RenderInspector();
}

#endif