#include "EnginePCH.h"
#include "AiController.h"

AiController::AiController(Object* owner)
	:Component(owner)
{
}

AiController* AiController::Create(Object* owner)
{
	AiController* Instance = new AiController(owner);
	
	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AiController::Initialize()
{
	return S_OK;
}

void AiController::Free()
{
	__super::Free();
}
