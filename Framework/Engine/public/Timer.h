#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Timer final:
    public Base
{
private:
	Timer();
	virtual ~Timer() = default;

public:
	static Timer* Create();
	HRESULT Initialize();
	void Free()override;

public:
	_float GetDeltaTime()const { return deltaTime; }
	void Update();

private:
	LARGE_INTEGER		frameTime{};
	LARGE_INTEGER		fixTime{};
	LARGE_INTEGER		lastTime{};
	LARGE_INTEGER		cpuTick{};

	_float				deltaTime{};

};

NS_END