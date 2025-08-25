#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Timer;
class TimerManager final:
    public Base
{
private:
    TimerManager();
    virtual ~TimerManager() = default;

public:
    static TimerManager* Create();
    HRESULT Initialize();
    void Free()override;

public:
    HRESULT AddTimer(const std::string& timerTag);
    void Update(const std::string& timerTag);
    _float GetDeltaTime(const std::string& timerTag);

private:
    Timer* FindTimer(const std::string& timerTag);

    std::unordered_map<std::string, Timer*> m_TimerMap;
};

NS_END