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
	timerMap.clear();

	return S_OK;
}

void TimerManager::Free()
{
	__super::Free();

	for (auto& pair : timerMap)
		Safe_Release(pair.second);
	timerMap.clear();
}

HRESULT TimerManager::AddTimer(const std::string& timerTag)
{
	Timer* timer = FindTimer(timerTag);

	if (timer)
		return E_FAIL;
	else
	{
		timer = Timer::Create();
		timerMap[timerTag] = timer;
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
	auto iter = timerMap.find(timerTag);

	if (iter != timerMap.end())
		return iter->second;
	else
		return nullptr;
}
