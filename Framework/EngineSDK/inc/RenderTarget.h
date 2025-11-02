#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Shader;
class VIBuffer;
class RenderTarget :
    public Base
{
private:
    RenderTarget();
    virtual ~RenderTarget() = default;

public:
    static RenderTarget* Create(_uint width, _uint height, DXGI_FORMAT format, _float4 clearColor);
    HRESULT Initialize(_uint width, _uint height, DXGI_FORMAT format, _float4 clearColor);

    HRESULT BindShaderResource(Shader* shader, const _string& constantName);
    ID3D11RenderTargetView* GetRTV()const { return m_pRTV; }
    ID3D11ShaderResourceView* GetSRV()const { return m_pSRV; }
    void Clear();

    void Free()override;

private:
    _float4 m_ClearColor{};

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    ID3D11Texture2D* m_pTexture2D = nullptr;
    ID3D11RenderTargetView* m_pRTV = nullptr;
    ID3D11ShaderResourceView* m_pSRV = nullptr;
};

NS_END