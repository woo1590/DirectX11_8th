#include "EnginePCH.h"
#include "FreecamComponent.h"

FreecamComponent::FreecamComponent(Object* owner)
	:Component(owner)
{
}

FreecamComponent::FreecamComponent(const FreecamComponent& prototype)
	:Component(prototype)
{
}

FreecamComponent* FreecamComponent::Create(Object* owner)
{
	FreecamComponent* Instance = new FreecamComponent(owner);
	
	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FreecamComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void FreecamComponent::Update(_float dt)
{
	/*키 입력 있어야되는데*/
}

void FreecamComponent::Free()
{
	__super::Free();
}
