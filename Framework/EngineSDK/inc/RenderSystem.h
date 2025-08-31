#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

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

    HRESULT Render();

    Renderer* GetRenderer()const { return m_pRenderer; }
    void Submit(std::vector<std::vector<RenderProxy>> proxies);
    void Free()override;

private:
    HRESULT RenderNonBlend();
    void Clear();

    std::vector<std::vector<RenderProxy>> m_CurrFrameProxies;

    Renderer* m_pRenderer = nullptr;
};

NS_END