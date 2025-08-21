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

HRESULT PrototypeManager::AddPrototype(_uint level, const _string& prototypeTag, Base* prototype)
{
	auto iter = prototypes[level].find(prototypeTag);

	if (iter != prototypes[level].end())	//원형이 존재하지 않는 경우에만 추가
		return E_FAIL;

	prototypes[level].emplace(prototypeTag, prototype);

	return S_OK;
}

Base* PrototypeManager::ClonePrototype(Prototype type, _uint level, const _string& prototypeTag, void* arg)
{
	Base* prototype = FindPrototype(level, prototypeTag);

	if (!prototype)
		return nullptr;	

	if (type == Prototype::Object)
		return static_cast<Object*>(prototype)->Clone(arg);
	else
		return nullptr; //컴포넌트 처리 해야됨 근데 컴포넌트를 왜 프로토타입으로 하는지 이해가 안감
}

void PrototypeManager::Clear(_uint level)
{
	for (auto& pair : prototypes[level])
		Safe_Release(pair.second);

	prototypes[level].clear();
}

Base* PrototypeManager::FindPrototype(_uint level, const _string& prototypeTag)
{
	auto iter = prototypes[level].find(prototypeTag);

	if (iter != prototypes[level].end())
		return iter->second;
	else
		return nullptr;
}
