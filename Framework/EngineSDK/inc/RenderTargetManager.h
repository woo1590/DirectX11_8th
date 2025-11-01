#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Shader;
class RenderTarget;
class RenderTargetManager :
    public Base
{
private:
    RenderTargetManager();
    virtual ~RenderTargetManager() = default;

public:
    static RenderTargetManager* Create();
    HRESULT AddRenderTarget(const _string& targetTag, _uint width, _uint height, DXGI_FORMAT format, _float4 clearColor);
    HRESULT AddMRT(const _string& mrtTag, const _string& targetTag);
    HRESULT BindShaderResource(Shader* shader, const _string& targetTag, const _string& constantName);
    HRESULT BeginMRT(const _string& mrtTag);
    HRESULT EndMRT();

    HRESULT Intialize();

    void Free()override;

private:
    RenderTarget* FindRenderTarget(const _string& targetTag);
    std::list<RenderTarget*>* FindMRT(const _string& mrtTag);

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
    std::unordered_map<_string, RenderTarget*> m_RenderTargets;
    std::unordered_map<_string, std::list<RenderTarget*>> m_MRTs;

    ID3D11RenderTargetView* m_pBackBuffer = nullptr;
    ID3D11DepthStencilView* m_pDSV = nullptr;
};

NS_END