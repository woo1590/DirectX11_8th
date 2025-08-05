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
    void Free()override;

private:
    std::vector<std::list<RenderProxy>> proxies;
};

NS_END