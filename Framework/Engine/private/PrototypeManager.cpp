#include "EnginePCH.h"
#include "PrototypeManager.h"

PrototypeManager::PrototypeManager()
{
}

PrototypeManager* PrototypeManager::Create()
{
	PrototypeManager* Instance = new PrototypeManager();
	

	return Instance;
}

HRESULT PrototypeManager::Initialize(_uint levelCnt)
{
	this->levelCnt = levelCnt;
	prototypes.resize(levelCnt);

	return S_OK;
}

void PrototypeManager::Free()
{
	__super::Free();

	for (auto& map : prototypes)
	{
		for (auto& pair : map)
		{
			Safe_Release(pair.second);
		}

		map.clear();
	}
	prototypes.clear();
}

void PrototypeManager::AddPrototype(_uint level, const _string& prototypeTag, Base* prototype)
{
	auto iter = prototypes[level].find(prototypeTag);

	if(iter==prototypes[level].end())	//원형이 존재하지 않는 경우에만 추가
		prototypes[level].insert({ prototypeTag, prototype });
}

Base* PrototypeManager::ClonePrototype(_uint level, const _string& prototypeTag, void* arg)
{
	Base* prototype = FindPrototype(level, prototypeTag);

	if (!prototype)
		return nullptr;
	else
	{

	}

	return prototype;
}

Base* PrototypeManager::FindPrototype(_uint level, const _string& prototypeTag)
{
	auto iter = prototypes[level].find(prototypeTag);

	if (iter != prototypes[level].end())
		return iter->second;
	else
		return nullptr;
}
