#include "EnginePCH.h"
#include "Timer.h"

Timer::Timer()
{
}

HRESULT Timer::Initialize()
{
	QueryPerformanceCounter(&frameTime);			// 1077
	QueryPerformanceCounter(&lastTime);			// 1085
	QueryPerformanceCounter(&fixTime);			// 1090

	QueryPerformanceFrequency(&cpuTick);		// cpu tick 값을 얻어오는 함수

	return S_OK;
}

void Timer::Update()
{
	QueryPerformanceCounter(&frameTime);			// 1500

	if (frameTime.QuadPart - fixTime.QuadPart >= cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&cpuTick);
		fixTime = frameTime;
	}

	deltaTime = (frameTime.QuadPart - lastTime.QuadPart) / static_cast<_float>(cpuTick.QuadPart);

	lastTime = frameTime;
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
