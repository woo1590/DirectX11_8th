#pragma once
#include "Base.h"
#include "RenderProxy.h"
#include "LightProxy.h"

NS_BEGIN(Engine)

class VIBuffer;
class Shader;
class ENGINE_DLL Renderer :
    public Base
{
private:
    Renderer();
    virtual ~Renderer() = default;

public:
    static Renderer* Create();
    HRESULT Initialize();

    HRESULT BeginFrame();

    HRESULT RenderPriority(const std::vector<RenderProxy>& proxies);
    HRESULT RenderNonBlend(const std::vector<RenderProxy>& proxies);
    HRESULT RenderLight(const std::vector<LightProxy>& proxies);
    HRESULT RenderCombined();
    HRESULT RenderBlend(const std::vector<RenderProxy>& proxies);
    HRESULT RenderUI(const std::vector<RenderProxy>& proxies);

    HRESULT ConnectConstantBuffer(Shader* shader);
    void Free()override;

private:
    HRESULT Initialize_DeferredTargets(D3D11_VIEWPORT viewPort);
    HRESULT DrawProxy(const RenderProxy& proxy);

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    ID3D11Buffer* m_pCBPerFrame = nullptr;
    ID3D11Buffer* m_pCBPerObject = nullptr;
    ID3D11Buffer* m_pCBBonePalatte = nullptr;
    ID3D11Buffer* m_pCBPerLight = nullptr;
    ID3D11Buffer* m_pCBPerUI = nullptr;

    /*for deferred rendering*/
    VIBuffer* m_pBuffer = nullptr;
    Shader* m_pShader = nullptr;
    _float4x4 m_WorldMatrix{};
    _float4x4 m_ViewMatrix{};
    _float4x4 m_ProjMatrix{};
};

NS_END