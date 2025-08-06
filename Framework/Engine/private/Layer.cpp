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
	objects.clear();

	return S_OK;
}

void Layer::Free()
{
	__super::Free();

	for (auto& object : objects)
		Safe_Release(object);
}

void Layer::Update(_float dt)
{
	for (const auto& object : objects)
		object->Update(dt);
}

void Layer::LateUpdate(_float dt)
{
	for (const auto& object : objects)
		object->LateUpdate(dt);
}
