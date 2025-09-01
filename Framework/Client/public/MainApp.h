#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class EngineCore;
NS_END

NS_BEGIN(Client)

class MainApp final :
    public Base
{
private:
    MainApp();
    virtual ~MainApp() = default;

public:
    static MainApp* Create(HINSTANCE hInstance, int nCmdShow);
    HRESULT Initialize(HINSTANCE hInstance, int nCmdShow);
    void Run();
    void Free()override;

private:
    bool InitWindow(HINSTANCE hInst, int nCmdShow);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hWnd{};
    HINSTANCE hInstance{};
    RECT windowSize{};
    MSG msg{};
    HACCEL hAccel{};

    bool isRunning = false;

    EngineCore* m_pEngineCore = nullptr;
};

NS_END