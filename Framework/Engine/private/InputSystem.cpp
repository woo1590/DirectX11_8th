#include "EnginePCH.h"
#include "InputSystem.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

InputSystem* InputSystem::Create()
{
	InputSystem* Instance = new InputSystem();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT InputSystem::Initialize()
{
	return S_OK;
}

void InputSystem::Free()
{
	__super::Free();

}
