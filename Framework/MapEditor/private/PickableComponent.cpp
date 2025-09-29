#include "MapEditorPCH.h"
#include "PickableComponent.h"

PickableComponent::PickableComponent(Object* owner)
	:Component(owner)
{
}

PickableComponent::PickableComponent(const PickableComponent& prototype)
	:Component(prototype)
{
}

HRESULT PickableComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT PickableComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void PickableComponent::Free()
{
	__super::Free();
}
