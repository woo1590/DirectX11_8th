#include "EnginePCH.h"
#include "FreecamComponent.h"

FreecamComponent::FreecamComponent(Object* owner)
	:Component(owner)
{
}

FreecamComponent* FreecamComponent::Create(Object* owner, InitDESC* arg)
{
	FreecamComponent* Instance = new FreecamComponent(owner);
	
	if (FAILED(Instance->Initialize(arg)))
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
