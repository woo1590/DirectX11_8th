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
	_float GetDeltaTime()const { return m_fDeltaTime; }
	void Update();

private:
	LARGE_INTEGER		m_iFrameTime{};
	LARGE_INTEGER		m_iFixTime{};
	LARGE_INTEGER		m_iLastTime{};
	LARGE_INTEGER		m_iCpuTick{};

	_float				m_fDeltaTime{};

};

NS_END