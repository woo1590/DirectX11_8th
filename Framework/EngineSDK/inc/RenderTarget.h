#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class RenderTarget :
    public Base
{
private:
    RenderTarget();
    virtual ~RenderTarget() = default;

public:
    static RenderTarget* Create();
    HRESULT Initialize();

    void Free()override;

private:
    ID3D11Texture2D* m_pTexture2D = nullptr;
    ID3D11RenderTargetView* m_pRTV = nullptr;
    ID3D11ShaderResourceView* m_pSRV = nullptr;
};

NS_END