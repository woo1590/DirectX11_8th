#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class EngineCore;

NS_END

NS_BEGIN(Client)

class IApp;
class Application :
    public Base
{
private:
    Application();
    virtual ~Application() = default;

public:
    static Application* Create(HINSTANCE hInstance, int nCmdShow);
    HRESULT Initialize(HINSTANCE hInstance, int nCmdShow);
    void Free()override;

    void Run();

private:
    bool InitWindow(HINSTANCE hInst, int nCmdShow);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND hWnd{};
    HINSTANCE hInstance{};
    RECT windowSize{};
    MSG msg{};
    HACCEL hAccel{};

    bool isRunning = false;

    std::vector<IApp*> apps;
    EngineCore* engine = nullptr;
};

NS_END