#pragma once
#include "Base.h"

NS_BEGIN(Engine)

struct RenderProxy
{

};

class Renderer;
class ENGINE_DLL RenderSystem :
    public Base
{
private:
    RenderSystem();
    virtual ~RenderSystem() = default;

public:
    static RenderSystem* Create();
    HRESULT Initialize();

    void RenderLoop();
    void Submit(std::list<RenderProxy> proxies){}
    void StartRenderThread();
    void StopRenderThread();

    void Free()override;

private:
    std::mutex m_RenderMutex;
    std::thread m_RenderThread;
    std::condition_variable m_Condition;
    std::atomic<_bool> m_isRunning = false;

    std::vector<std::list<RenderProxy>> m_CurrFrameProxies;
};

NS_END