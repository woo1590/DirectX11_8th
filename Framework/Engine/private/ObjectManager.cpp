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
	m_Layers.clear();
	m_Layers.resize(levelCnt);

	return S_OK;
}

void ObjectManager::Free()
{
	__super::Free();

	for (auto& map : m_Layers)
	{
		for (auto& pair : map)
			Safe_Release(pair.second);

		map.clear();
	}

	m_Layers.clear();
}

void ObjectManager::Update(_float dt)
{
	for (const auto& map : m_Layers)
	{
		for (const auto& pair : map)
		{
			if(pair.second->IsUpdatable())
				pair.second->Update(dt);
		}
	}
}

void ObjectManager::LateUpdate(_float dt)
{
	for (const auto& map : m_Layers)
	{
		for (const auto& pair : map)
		{
			if (pair.second->IsUpdatable())
				pair.second->LateUpdate(dt);
		}
	}
}

HRESULT ObjectManager::AddObject(_uint prototypeLevelID, const _string& prototypeTag, _uint layerLevelID, const _string& layerTag, InitDESC* arg)
{
	Object* object = static_cast<Object*>(EngineCore::GetInstance()->ClonePrototype(Prototype::Object, prototypeLevelID, prototypeTag, arg));
	if (!object)
		return E_FAIL;

	Layer* layer = FindLayer(layerLevelID, layerTag);
	if(!layer)
	{
		layer = Layer::Create();
		m_Layers[layerLevelID].emplace(layerTag, layer);
	}

	layer->AddObject(object);

	return S_OK;
}

void ObjectManager::Clear(_uint levelID)
{
	if (levelID >= m_Layers.size())
		return;

	for (auto& pair : m_Layers[levelID])
		Safe_Release(pair.second);
	m_Layers[levelID].clear();
}

Layer* ObjectManager::FindLayer(_uint layerLevel, const _string& layerTag)
{
	auto iter = m_Layers[layerLevel].find(layerTag);

	if (iter == m_Layers[layerLevel].end())
		return nullptr;

	return iter->second;
}
