#include "pch.h"
#include "Application.h"
#include "EngineCore.h"
#include "MainApp.h"

Application::Application()
{
}

Application* Application::Create(HINSTANCE hInstance, int nCmdShow)
{
	Application* Instance = new Application();

	if (FAILED(Instance->Initialize(hInstance, nCmdShow)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Application::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    if (!InitWindow(hInstance, nCmdShow))
        return E_FAIL;

    EngineDESC desc{};
    desc.hWnd = this->hWnd;
    desc.winSizeX = WinSizeX;
    desc.winSizeY = WinSizeY;

    engine = EngineCore::GetInstance();
    if (FAILED(engine->Initialize(desc)))
    {
        MSG_BOX("Create Failed : EngineCore");
        return E_FAIL;
    }

    isRunning = true;

	return S_OK;
}

void Application::Free()
{
    __super::Free();

    engine->DestroyInstance();
}

void Application::Run()
{
    while (isRunning)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message) break;

            if (!TranslateAccelerator(msg.hwnd, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        //game loop
    }
}

bool Application::InitWindow(HINSTANCE hInst, int nCmdShow)
{
    hInstance = hInst;

    const wchar_t* CLASS_NAME = L"Direct11_Project";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        OutputDebugString(L"[Engine] RegisterClassEx failed\n");
        return false;
    }

    windowSize = { 0, 0, WinSizeX, WinSizeY };

    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = windowSize.right - windowSize.left;
    int windowHeight = windowSize.bottom - windowSize.top;

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    hWnd = CreateWindowW(
        CLASS_NAME, L"My Game Window",
        WS_OVERLAPPEDWINDOW,
        x, y,
        windowWidth,
        windowHeight,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd) {
        OutputDebugString(L"[Engine] CreateWindow failed\n");
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return true;
}

LRESULT Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}
