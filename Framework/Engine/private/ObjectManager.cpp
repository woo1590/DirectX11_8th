#include "EnginePCH.h"
#include "EngineCore.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Layer.h"

ObjectManager::ObjectManager()
{
}

ObjectManager* ObjectManager::Create(_uint levelCnt)
{
	ObjectManager* Instance = new ObjectManager();

	if (FAILED(Instance->Initialize(levelCnt)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ObjectManager::Initialize(_uint levelCnt)
{
	layers.clear();
	layers.resize(levelCnt);

	return S_OK;
}

void ObjectManager::Free()
{
	__super::Free();

	for (auto& map : layers)
	{
		for (auto& pair : map)
			Safe_Release(pair.second);

		map.clear();
	}

	layers.clear();
}

void ObjectManager::Update(_float dt)
{
	for (const auto& map : layers)
	{
		for (const auto& pair : map)
			pair.second->Update(dt);
	}
}

void ObjectManager::LateUpdate(_float dt)
{
	for (const auto& map : layers)
	{
		for (const auto& pair : map)
			pair.second->LateUpdate(dt);
	}
}

HRESULT ObjectManager::AddObject(_uint prototypeLevelID, const _string& prototypeTag, _uint layerLevelID, const _string& layerTag, void* arg)
{
	Object* object = static_cast<Object*>(EngineCore::GetInstance()->ClonePrototype(Prototype::Object, prototypeLevelID, prototypeTag, arg));
	if (!object)
		return E_FAIL;

	Layer* layer = FindLayer(layerLevelID, layerTag);
	if(!layer)
	{
		layer = Layer::Create();
		layers[layerLevelID].emplace(layerTag, layer);
	}

	layer->AddObject(object);

	return S_OK;
}

void ObjectManager::Clear(_uint levelID)
{
	if (levelID >= layers.size())
		return;

	for (auto& pair : layers[levelID])
		Safe_Release(pair.second);
	layers[levelID].clear();
}

Layer* ObjectManager::FindLayer(_uint layerLevel, const _string& layerTag)
{
	auto iter = layers[layerLevel].find(layerTag);

	if (iter == layers[layerLevel].end())
		return nullptr;

	return iter->second;
}
