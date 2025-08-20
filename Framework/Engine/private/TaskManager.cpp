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
	Start();
	return S_OK;
}

void TaskManager::Free()
{
	__super::Free();
	Stop();
}

void TaskManager::Start()
{
	_uint threadCnt = std::thread::hardware_concurrency();
	if (!threadCnt)		//¿À·ù ½Ã fallback
		threadCnt = 4;

	for (int i = 0; i < threadCnt; ++i)
		workers.emplace_back(&TaskManager::Worker, this);
}

void TaskManager::Stop()
{
	{
		std::unique_lock<std::mutex> lock(queueMutex);
		stop = true;
	}
	condition.notify_all();

	for (std::thread& worker : workers)
	{
		if (worker.joinable())
			worker.join();
	}

	workers.clear();
}

void TaskManager::Worker()
{
	while (true)
	{
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(queueMutex);

			condition.wait(lock, [this] {return stop || !tasks.empty(); });

			if (stop && tasks.empty()) return;

			task = std::move(tasks.front());
			tasks.pop();
		}

		task();
	}
}
