#include "EnginePCH.h"
#include "TaskManager.h"

TaskManager::TaskManager()
{
}

TaskManager* TaskManager::Create()
{
	TaskManager* Instance = new TaskManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TaskManager::Initialize()
{
	return S_OK;
}

void TaskManager::Free()
{
	__super::Free();
}
