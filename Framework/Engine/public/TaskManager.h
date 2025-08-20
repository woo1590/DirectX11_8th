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

    template<typename F>
    std::future<typename std::invoke_result<F>::type> Enqueue(F&& task)
    {
        using ReturnType = typename std::invoke_result<F>::type;

        auto packageTask = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<F>(task));
        auto future = packageTask->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push([packageTask] {(*packageTask)(); });
        }

        condition.notify_one();
        
        return future;
    }
private:
    void Start();
    void Stop();
    void Worker();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    _bool stop = false;
};

NS_END