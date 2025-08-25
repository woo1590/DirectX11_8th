#include "EnginePCH.h"
#include "Timer.h"

Timer::Timer()
{
}

HRESULT Timer::Initialize()
{
	QueryPerformanceCounter(&m_iFrameTime);			// 1077
	QueryPerformanceCounter(&m_iLastTime);			// 1085
	QueryPerformanceCounter(&m_iFixTime);			// 1090

	QueryPerformanceFrequency(&m_iCpuTick);		// cpu tick 값을 얻어오는 함수

	return S_OK;
}

void Timer::Update()
{
	QueryPerformanceCounter(&m_iFrameTime);			// 1500

	if (m_iFrameTime.QuadPart - m_iFixTime.QuadPart >= m_iCpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_iCpuTick);
		m_iFixTime = m_iFrameTime;
	}

	m_fDeltaTime = (m_iFrameTime.QuadPart - m_iLastTime.QuadPart) / static_cast<_float>(m_iCpuTick.QuadPart);

	m_iLastTime = m_iFrameTime;
}

Timer* Timer::Create()
{
	Timer* Instance = new Timer();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

void Timer::Free()
{
	__super::Free();
}
