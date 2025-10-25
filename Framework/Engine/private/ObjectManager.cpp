#include "EnginePCH.h"
#include "EngineCore.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Layer.h"

ObjectManager::ObjectManager()
{
}

ObjectManager* ObjectManager::Create(_uint numLevels)
{
	ObjectManager* Instance = new ObjectManager();

	if (FAILED(Instance->Initialize(numLevels)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ObjectManager::Initialize(_uint numLevels)
{
	m_Layers.clear();
	m_Layers.resize(numLevels);

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

void ObjectManager::PriorityUpdate(_float dt)
{
	for (const auto& map : m_Layers)
	{
		for (const auto& pair : map)
		{
			if (pair.second->IsUpdatable())
				pair.second->PriorityUpdate(dt);
		}
	}
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

	RemoveDeadObjects();
}

void ObjectManager::RemoveDeadObjects()
{
	for (const auto& map : m_Layers)
	{
		for (const auto& pair : map)
			pair.second->RemoveDeadObjects();
	}
}

HRESULT ObjectManager::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	/* 매개 인자로 들어오는 벡터를 직접 넘기지 말고 
	   여기서 벡터 하나씩 생성하고 나서 멀티 스레드로 넘겨도 될듯? 
	   일단 나중에 -> 렌더링 진도 다 끝나고나서 */

	for (const auto& map : m_Layers)
	{
		for (const auto& pair : map)
		{
			if (FAILED(pair.second->ExtractRenderProxies(proxies)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT ObjectManager::AddObject(_uint prototypeLevelID, const _string& prototypeTag, _uint layerLevelID, const _string& layerTag, InitDESC* arg, Object** out)
{
	Object* object = static_cast<Object*>(EngineCore::GetInstance()->ClonePrototype(prototypeLevelID, prototypeTag, arg));
	if (!object)
	{
		if(out)
			*out = nullptr;

		return E_FAIL;
	}

	Layer* layer = FindLayer(layerLevelID, layerTag);
	if(!layer)
	{
		layer = Layer::Create();
		m_Layers[layerLevelID].emplace(layerTag, layer);
	}

	layer->AddObject(object);

	if (out)
		*out = object;

	return S_OK;
}

std::unordered_map<_string, Layer*>& ObjectManager::GetLayers(_uint levelID)
{
	return m_Layers[levelID];
}

const std::list<Object*>& ObjectManager::GetObjects(_uint layerLevel, const _string& layerTag)
{
	static std::list<Object*> emptyList;
	Layer* layer = FindLayer(layerLevel, layerTag);

	if (!layer)
		return emptyList;

	return layer->GetObjects();
}

Object* ObjectManager::GetFrontObject(_uint layerLevel, const _string& layerTag)
{
	Layer* layer = FindLayer(layerLevel, layerTag);
	if (!layer)
		return nullptr;

	return layer->GetFrontObject();
}

Object* ObjectManager::GetObjectByInstanceTag(_uint layerLevel, const _string& layerTag, const _string& instanceTag)
{
	Layer* layer = FindLayer(layerLevel, layerTag);
	if (!layer)
		return nullptr;

	return layer->GetObjectByInstanceTag(instanceTag);
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
