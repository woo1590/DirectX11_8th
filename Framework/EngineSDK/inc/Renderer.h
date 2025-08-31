#pragma once
#include "Base.h"
#include "RenderProxy.h"

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

    HRESULT BeginFrame();
    HRESULT DrawProxy(const RenderProxy& proxy);
    HRESULT EndFrame() { return S_OK; }

    HRESULT ConnectConstantBuffer(ID3DX11Effect* pEffect);
    void Free()override;

private:
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    ID3D11Buffer* m_pCBPerFrame = nullptr;
    ID3D11Buffer* m_pCBPerObject = nullptr;

};

NS_END