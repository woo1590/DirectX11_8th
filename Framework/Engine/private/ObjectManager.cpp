#include "EnginePCH.h"
#include "ObjectManager.h"
#include "Layer.h"

ObjectManager::ObjectManager()
{
}

ObjectManager* ObjectManager::Create()
{
	ObjectManager* Instance = new ObjectManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ObjectManager::Initialize()
{
	layers.clear();

	return S_OK;
}

void ObjectManager::Free()
{
	__super::Free();

	for (auto& pair : layers)
		Safe_Release(pair.second);

	layers.clear();
}
