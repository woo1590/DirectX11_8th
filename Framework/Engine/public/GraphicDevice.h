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
    static GraphicDevice* Create(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY);
    HRESULT Initialize(HWND hWnd, WinMode winMode, _uint winSizeX, _uint winSizeY);
    void Free()override;

public:
    HRESULT ClearBackBufferView();
    HRESULT ClearDepthStencilView();
    HRESULT Present();

    ID3D11Device* GetDevice()const { return device; }
    ID3D11DeviceContext* GetDeviceContext()const { return deviceContext; }

private:
    HRESULT InitSwapChain(HWND hWnd, WinMode winMode ,_uint winSizeX, _uint winSizeY);
    HRESULT InitBackBufferRTV(_uint winSizeX, _uint winSizeY);
    HRESULT InitDSV(_uint winSizeX, _uint winSizeY);

    ID3D11Device* device = nullptr;                     //gpu 자원 생성
    ID3D11DeviceContext* deviceContext = nullptr;       //장치에 바인딩, 실제 그리기 작업을 담당
    IDXGISwapChain* swapChain = nullptr;                //스왑체인 인터페이스
    ID3D11RenderTargetView* backBufferRTV = nullptr;    //백버퍼 rtv
    ID3D11DepthStencilView* DSV = nullptr;              //depth - stencil 

    _float4 clearColor{ 0.f,0.f,1.f,1.f };
};

NS_END