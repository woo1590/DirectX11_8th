#include "EnginePCH.h"
#include "TimerManager.h"
#include "Timer.h"

TimerManager::TimerManager()
{
}

TimerManager* TimerManager::Create()
{
	TimerManager* Instance = new TimerManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TimerManager::Initialize()
{
	m_TimerMap.clear();

	return S_OK;
}

void TimerManager::Free()
{
	__super::Free();

	for (auto& pair : m_TimerMap)
		Safe_Release(pair.second);
	m_TimerMap.clear();
}

HRESULT TimerManager::AddTimer(const std::string& timerTag)
{
	Timer* timer = FindTimer(timerTag);

	if (timer)
		return E_FAIL;
	else
	{
		timer = Timer::Create();
		m_TimerMap[timerTag] = timer;
	}

	return S_OK;
}

void TimerManager::Update(const std::string& timerTag)
{
	Timer* timer = FindTimer(timerTag);

	if (timer)
		timer->Update();
	else
		return;
}

_float TimerManager::GetDeltaTime(const std::string& timerTag)
{
	Timer* timer = FindTimer(timerTag);

	if (timer)
		return timer->GetDeltaTime();
	else
		return 0.f;
}

Timer* TimerManager::FindTimer(const std::string& timerTag)
{
	auto iter = m_TimerMap.find(timerTag);

	if (iter != m_TimerMap.end())
		return iter->second;
	else
		return nullptr;
}
