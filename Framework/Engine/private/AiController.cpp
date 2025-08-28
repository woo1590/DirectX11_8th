#include "EnginePCH.h"
#include "AiController.h"

AiController::AiController(Object* owner)
	:Component(owner)
{
}

AiController::AiController(const AiController& prototype)
	:Component(prototype)
{
}

AiController* AiController::Create(Object* owner)
{
	AiController* Instance = new AiController(owner);
	
	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AiController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT AiController::Initialize(InitDESC* arg)
{
	return S_OK;
}

void AiController::Free()
{
	__super::Free();
}
