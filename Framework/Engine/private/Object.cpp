#include "EnginePCH.h"
#include "Object.h"
#include "Component.h"

Object::Object()
{
}

HRESULT Object::Initialize()
{
	components.clear();
	componentMap.clear();

	return S_OK;
}

void Object::Update(_float dt)
{
	for (const auto& comp : components)
		comp->Update(dt);
}

void Object::LateUpdate(_float dt)
{
	for (const auto& comp : components)
		comp->LateUpdate(dt);
}

void Object::Free()
{
	for (auto& comp : components)
		Safe_Release(comp);

	for (auto& [key, comp] : componentMap)
		Safe_Release(comp);

	components.clear();
	componentMap.clear();
}
