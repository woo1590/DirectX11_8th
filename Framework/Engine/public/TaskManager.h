#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class TaskManager final:
    public Base
{
private:
    TaskManager();
    virtual ~TaskManager() = default;

public:
    static TaskManager* Create();
    HRESULT Initialize();
    void Free()override;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    _bool stop = false;
};

NS_END