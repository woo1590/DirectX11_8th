#pragma once
#include "Base.h"
#include "RenderProxy.h"
#include "LightProxy.h"

NS_BEGIN(Engine)

class ENGINE_DLL Renderer :
    public Base
{
private:
    Renderer();
    virtual ~Renderer() = default;

public:
    static Renderer* Create();
    HRESULT Initialize();

    HRESULT BeginFrame(std::vector<LightProxy>& lights);
    HRESULT EndFrame() { return S_OK; }

    HRESULT RenderPriority(const std::vector<RenderProxy>& proxies);
    HRESULT RenderNonBlend(const std::vector<RenderProxy>& proxies);
    HRESULT RenderBlend(const std::vector<RenderProxy>& proxies);
    HRESULT RenderUI(const std::vector<RenderProxy>& proxies);
    HRESULT RenderDebug(const std::vector<RenderProxy>& proxies);

    HRESULT ConnectConstantBuffer(ID3DX11Effect* pEffect);
    void Free()override;

private:
    HRESULT DrawProxy(const RenderProxy& proxy,const _string& passTag);

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    ID3D11Buffer* m_pCBPerFrame = nullptr;
    ID3D11Buffer* m_pCBPerObject = nullptr;
    ID3D11Buffer* m_pCBBonePalatte = nullptr;
    ID3D11Buffer* m_pCBPerLightViewProj = nullptr;
    ID3D11Buffer* m_pCBPerUI = nullptr;
};

NS_END