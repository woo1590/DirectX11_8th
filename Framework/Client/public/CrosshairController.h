#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class Object;
NS_END

NS_BEGIN(Client)

class CrosshairController :
    public Base
{
    enum class CrosshairDir { Up, Down, Right, Left, Count };
private:
    CrosshairController();
    virtual ~CrosshairController() = default;

public:
    static CrosshairController* Create();
    HRESULT Initialize();
    void Update(_float dt);

    void Spread(_bool spread) { m_IsSpread = spread; }
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);
    void Free()override;

private:
    std::vector<Object*> m_Crosshairs;
    _bool m_IsSpread = false;
    _float m_fLerpScale = 12.f;
    _float m_fMinSpread{};
    _float m_fMaxSpread{};
};

NS_END