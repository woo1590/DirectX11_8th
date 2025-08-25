#include "EnginePCH.h"
#include "Layer.h"
#include "Object.h"

Layer::Layer()
{
}

Layer* Layer::Create()
{
	Layer* Instance = new Layer();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Layer::Initialize()
{
	m_Objects.clear();

	return S_OK;
}

void Layer::Free()
{
	__super::Free();

	for (auto& object : m_Objects)
		Safe_Release(object);
}

void Layer::AddObject(Object* object)
{
	m_Objects.push_back(object);
}

void Layer::Update(_float dt)
{
	for (const auto& object : m_Objects)
		object->Update(dt);
}

void Layer::LateUpdate(_float dt)
{
	for (const auto& object : m_Objects)
		object->LateUpdate(dt);
}
