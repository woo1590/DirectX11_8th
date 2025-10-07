#include "EnginePCH.h"
#include "NavigationSystem.h"
#include "NavigationComponent.h"

NavigationSystem::NavigationSystem()
{
}

NavigationSystem* NavigationSystem::Create()
{
	NavigationSystem* Instance = new NavigationSystem();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavigationSystem::Initialize()
{
	return S_OK;
}

void NavigationSystem::RegisterNavigation(NavigationComponent* component)
{
	m_NavComponents.push_back(component);
	component->AddRef();
}

void NavigationSystem::UnRegisterNavigation(NavigationComponent* component)
{
	_uint currSize = m_NavComponents.size();

	m_NavComponents.remove(component);
	if (currSize != m_NavComponents.size())
		Safe_Release(component);
}

void NavigationSystem::Free()
{
	__super::Free();

	for (auto& component : m_NavComponents)
		Safe_Release(component);
	m_NavComponents.clear();
}
