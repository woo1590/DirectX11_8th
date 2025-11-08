#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Shader;
class ENGINE_DLL DebugRenderer :
    public Base
{
private:
    DebugRenderer();
    virtual ~DebugRenderer() = default;

public:
    static DebugRenderer* Create();
    HRESULT Initialize();

    HRESULT BeginFrame();
    HRESULT NavMeshDebug(const std::vector<RenderProxy>& proxies);
    HRESULT ColliderDebug(const std::vector<RenderProxy>& proxies);

    void Free()override;

private:
    HRESULT DrawDebugProxy(const RenderProxy& proxy);

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    ID3D11Buffer* m_pCBPerFrame = nullptr;
    ID3D11Buffer* m_pCBPerDraw = nullptr;

    Shader* m_pCellDebugShader = nullptr;
};

NS_END