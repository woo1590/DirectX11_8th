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

	for (int i = 0; i < threadCnt-1; ++i)
		m_Workers.emplace_back(&TaskManager::Worker, this);
}

void TaskManager::Stop()
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_Stop = true;
	}
	m_Condition.notify_all();

	for (std::thread& worker : m_Workers)
	{
		if (worker.joinable())
			worker.join();
	}

	m_Workers.clear();
}

void TaskManager::Worker()
{
	while (true)
	{
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);

			m_Condition.wait(lock, [this] {return m_Stop || !m_Tasks.empty(); });

			if (m_Stop && m_Tasks.empty()) return;

			task = std::move(m_Tasks.front());
			m_Tasks.pop();
		}

		task();
	}
}
