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
    auto Enqueue(F&& task) -> std::future<typename std::invoke_result<F>::type>
    {
        using ReturnType = typename std::invoke_result<F>::type;

        auto packageTask = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<F>(task));
        auto future = packageTask->get_future();

        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Tasks.emplace([packageTask] {(*packageTask)(); });
        }

        m_Condition.notify_one();
        return future;

        //task로 등록할 수 있는 함수는 void() 형 함수만 가능
        //람다로 감싸서 void()형으로 전달, 매개변수가 있을경우 캡쳐를 통해서 전달
        //task의 반환타입을 invoke_result로 확인 -> 해당 반환형에 대한 핸들을 가지는 future객체 반환
    }

private:
    void Start();
    void Stop();
    void Worker();

    std::vector<std::thread> m_Workers;
    std::queue<std::function<void()>> m_Tasks;
    std::mutex m_QueueMutex;
    std::condition_variable m_Condition;
    _bool m_Stop = false;

};

NS_END