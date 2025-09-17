#include "MapEditorPCH.h"
#include "MainApp.h"
#include "EngineCore.h"

MainApp::MainApp()
{
}

MainApp* MainApp::Create(HINSTANCE hInstance, int nCmdShow)
{
    MainApp* Instance = new MainApp();

    if (FAILED(Instance->Initialize(hInstance, nCmdShow)))
    {
        Instance = nullptr;
    }

    return Instance;
}

HRESULT MainApp::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    if (!InitWindow(hInstance, nCmdShow))
        return E_FAIL;

    EngineDESC desc{};
    desc.hWnd = this->hWnd;
    desc.winMode = WinMode::Win;
    desc.winSizeX = WinSizeX;
    desc.winSizeY = WinSizeY;
    desc.levelCnt = static_cast<_uint>(LevelID::Count);

    m_pEngineCore = EngineCore::GetInstance();
    if (FAILED(m_pEngineCore->Initialize(desc)))
    {
        MSG_BOX("Create Failed : EngineCore");
        return E_FAIL;
    }

    m_pEngineCore->AddTimer("Timer_Default");
    m_pEngineCore->AddTimer("Timer_144fps");

   // m_pEngineCore->ChangeLevel(ENUM_CLASS(LevelID::Editor), ImportLevel::Create());

    isRunning = true;

    return S_OK;
}

void MainApp::Run()
{
    _float timeAcc = 0.f;

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

        m_pEngineCore->UpdateTimer("Timer_Default");
        timeAcc += m_pEngineCore->GetDeltaTime("Timer_Default");

        if (timeAcc >= 1.f / 144.f)
        {
            m_pEngineCore->UpdateTimer("Timer_144fps");
            _float dt = m_pEngineCore->GetDeltaTime("Timer_144fps");

            dt = std::clamp(dt, 0.f, 0.03f);
            m_pEngineCore->Tick(dt);

            timeAcc = 0.f;
        }
    }
}

void MainApp::Free()
{
    m_pEngineCore->DestroyInstance();
}

bool MainApp::InitWindow(HINSTANCE hInst, int nCmdShow)
{
    hInstance = hInst;

    const wchar_t* CLASS_NAME = L"Direct11_Project";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = EngineCore::WndProc;
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
        OutputDebugString(L"RegisterClassEx failed\n");
        return false;
    }

    windowSize = { 0, 0, WinSizeX, WinSizeY };

    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = windowSize.right - windowSize.left;
    int windowHeight = windowSize.bottom - windowSize.top;

    int x = static_cast<_int>(WinSizeX / 4);
    int y = static_cast<_int>(WinSizeY / 4);

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

    //ShowCursor(false);
    //ClipCursor(&windowSize);

    return true;
}