#include "EnginePCH.h"
#include "Object.h"
#include "TransformComponent.h"

Object::Object()
{
}

Object::Object(const Object& prototype)
{

}

HRESULT Object::Initialize_Prototype()
{
	m_Components.clear();
	m_ComponentMap.clear();

	return S_OK;
}

HRESULT Object::Initialize(InitDESC* arg)
{
	m_pTransform = AddComponent<TransformComponent>(arg);
	if (!m_pTransform)
		return E_FAIL;

	m_pTransform->AddRef();

	return S_OK;
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
