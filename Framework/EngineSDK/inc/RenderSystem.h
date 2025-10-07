#pragma once
#include "Base.h"
#include "RenderProxy.h"
#include "LightProxy.h"

NS_BEGIN(Engine)



class DebugRenderer;
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

    HRESULT RenderLoop();

    void Submit(std::vector<std::vector<RenderProxy>> proxies, std::vector<LightProxy> lights);
    Renderer* GetRenderer()const { return m_pRenderer; }

    void Free()override;

private:
    void Clear();

    std::vector<std::vector<RenderProxy>> m_CurrFrameProxies;
    std::vector<LightProxy> m_CurrFrameLights;

    Renderer* m_pRenderer = nullptr;
    DebugRenderer* m_pDebugRenderer = nullptr;
};

NS_END