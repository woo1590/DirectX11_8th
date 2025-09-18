#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class GraphicDevice :
    public Base
{
private:
    GraphicDevice();
    virtual ~GraphicDevice() = default;

public:
    static GraphicDevice* Create(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY, D3D11_VIEWPORT* pViewportDesc);
    HRESULT Initialize(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY, D3D11_VIEWPORT* pViewportDesc);
    void Free()override;

public:
    HRESULT ClearBackBufferView();
    HRESULT ClearDepthStencilView();
    HRESULT Present();

    ID3D11Device* GetDevice()const { return m_pDevice; }
    ID3D11DeviceContext* GetDeviceContext()const { return m_pDeviceContext; }

private:
    HRESULT InitSwapChain(HWND hWnd, WinMode winMode ,_uint winSizeX, _uint winSizeY);
    HRESULT InitBackBufferRTV(_uint winSizeX, _uint winSizeY);
    HRESULT InitDSV(_uint winSizeX, _uint winSizeY);

    ID3D11Device* m_pDevice = nullptr;                     //gpu 자원 생성
    ID3D11DeviceContext* m_pDeviceContext = nullptr;       //장치에 바인딩, 실제 그리기 작업을 담당
    IDXGISwapChain* m_pSwapChain = nullptr;                //스왑체인 인터페이스
    ID3D11RenderTargetView* m_BackBufferRTV = nullptr;    //백버퍼 rtv
    ID3D11DepthStencilView* m_pDSV = nullptr;              //depth - stencil 

    _float4 m_pClearColor{ 34 / 255.f, 38 / 255.f, 43 / 255.f, 1.0f};
};

NS_END