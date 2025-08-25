#include "EnginePCH.h"
#include "PrototypeManager.h"
#include "Object.h"
#include "Component.h"

PrototypeManager::PrototypeManager()
{
}

PrototypeManager* PrototypeManager::Create(_uint levelCnt)
{
	PrototypeManager* Instance = new PrototypeManager();
	
	if (FAILED(Instance->Initialize(levelCnt)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PrototypeManager::Initialize(_uint levelCnt)
{
	this->m_iLevelCnt = levelCnt;
	m_Prototypes.resize(levelCnt);

	return S_OK;
}

void PrototypeManager::Free()
{
	__super::Free();

	for (auto& map : m_Prototypes)
	{
		for (auto& pair : map)
		{
			Safe_Release(pair.second);
		}

		map.clear();
	}
	m_Prototypes.clear();
}

HRESULT PrototypeManager::AddPrototype(_uint level, const _string& prototypeTag, Base* prototype)
{
	auto iter = m_Prototypes[level].find(prototypeTag);

	if (iter != m_Prototypes[level].end())	//원형이 존재하지 않는 경우에만 추가
		return E_FAIL;

	m_Prototypes[level].emplace(prototypeTag, prototype);

	return S_OK;
}

Base * PrototypeManager::ClonePrototype(Prototype type, _uint level, const _string& prototypeTag, InitDESC* arg)
{
	Base* prototype = FindPrototype(level, prototypeTag);

	if (!prototype)
		return nullptr;	

	if (type == Prototype::Object)
		return static_cast<Object*>(prototype)->Clone(arg);
	else
		return static_cast<Component*>(prototype)->Clone(arg);
}

void PrototypeManager::Clear(_uint level)
{
	for (auto& pair : m_Prototypes[level])
		Safe_Release(pair.second);

	m_Prototypes[level].clear();
}

Base* PrototypeManager::FindPrototype(_uint level, const _string& prototypeTag)
{
	auto iter = m_Prototypes[level].find(prototypeTag);

	if (iter != m_Prototypes[level].end())
		return iter->second;
	else
		return nullptr;
}
