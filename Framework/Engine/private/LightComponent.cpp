#include "EnginePCH.h"
#include "LightComponent.h"

LightComponent::LightComponent(Object* owner)
	:Component(owner)
{
}

LightComponent::LightComponent(const LightComponent& prototype)
	:Component(prototype)
{
}

LightComponent* LightComponent::Create(Object* owner)
{
	LightComponent* Instance = new LightComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT LightComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void LightComponent::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI
void LightComponent::RenderInspector()
{

}
#endif