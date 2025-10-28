#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class RenderTarget;
class RenderTargetManager :
    public Base
{
private:
    RenderTargetManager();
    virtual ~RenderTargetManager() = default;

public:
    static RenderTargetManager* Create();
    HRESULT Intialize();

    void Free()override;

private:
    std::unordered_map<_string, RenderTarget*> m_RenderTargets;
    std::unordered_map<_string, std::list<RenderTarget*>> m_MRTs;
};

NS_END