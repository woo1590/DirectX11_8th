#include "EnginePCH.h"
#include "NavigationComponent.h"
#include "NavMesh.h"

NavigationComponent::NavigationComponent(Object* owner)
	:Component(owner)
{
}

NavigationComponent::NavigationComponent(const NavigationComponent& prototype)
	:Component(prototype)
{
}

NavigationComponent* NavigationComponent::Create(Object* owner)
{
	NavigationComponent* Instance = new NavigationComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavigationComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT NavigationComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void NavigationComponent::Free()
{
}
