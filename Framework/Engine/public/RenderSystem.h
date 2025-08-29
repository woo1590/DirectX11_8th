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
    void Submit(std::vector<std::vector<RenderProxy>> proxies);
    void Free()override;

private:
    HRESULT RenderNonBlend();
    void Clear();

    std::vector<std::vector<RenderProxy>> m_CurrFrameProxies;

    Renderer* m_pRenderer = nullptr;

    ID3D11Buffer* m_pCBPerFrame = nullptr;
    ID3D11Buffer* m_pCBPerLight = nullptr;  
    ID3D11Buffer* m_pCBPerObject = nullptr;

};

NS_END