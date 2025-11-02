#include "pch.h"
#include "MainApp.h"
#include "EngineCore.h"
#include "LoadingLevel.h"
#include "VIBufferCube.h"
#include "VIBufferQuad.h"

//object
#include "BackGround.h"
#include "Socket.h"

MainApp::MainApp()
{
    // È÷È÷ ¿ÀÁÜ¹ß½Î
}

MainApp* MainApp::Create(HINSTANCE hInstance, int nCmdShow)
{
    MainApp* Instance = new MainApp();

    if (FAILED(Instance->Initialize(hInstance,nCmdShow)))
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
    desc.levelCnt = ENUM_CLASS(LevelID::Count);
    desc.numCollisionFilter = ENUM_CLASS(ColliderFilter::Count);

    m_pEngineCore = EngineCore::GetInstance();
    if (FAILED(m_pEngineCore->Initialize(desc)))
    {
        MSG_BOX("Create Failed : EngineCore");
        return E_FAIL;
    }

    m_pEngineCore->AddTimer("Timer_Default");
    m_pEngineCore->AddTimer("Timer_144fps");

    if (FAILED(LoadStaticLevel()))
        return E_FAIL;
    AddColliderFilterGroup();

    m_pEngineCore->ChangeLevel(ENUM_CLASS(LevelID::Loading), LoadingLevel::Create(LevelID::Logo));

    isRunning = true;

    return S_OK;
}

void MainApp::Run()
{
    _float timeAcc = 0.f;

    while (isRunning)
    {

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                isRunning = false;
                break;
            }

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

HRESULT MainApp::LoadStaticLevel()
{
    /*Load Shader*/
    {
        if (FAILED(m_pEngineCore->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxTex.hlsl", "Shader_VtxTex",
            VTXTEX::elements, VTXTEX::numElement)))
            return E_FAIL;

        if (FAILED(m_pEngineCore->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxCube.hlsl", "Shader_VtxCube",
            VTXCUBE::elements, VTXCUBE::numElement)))
            return E_FAIL;

        if (FAILED(m_pEngineCore->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxNorTex.hlsl", "Shader_VtxNorTex",
            VTXNORTEX::elements, VTXNORTEX::numElement)))
            return E_FAIL;

        if (FAILED(m_pEngineCore->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxMesh.hlsl", "Shader_VtxMesh",
            VTXMESH::elements, VTXMESH::numElement)))
            return E_FAIL;

        if (FAILED(m_pEngineCore->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxSkinnedMesh.hlsl", "Shader_VtxSkinnedMesh",
            VTXSKINNEDMESH::elements, VTXSKINNEDMESH::numElement)))
            return E_FAIL;
    }
    /*Load Buffer*/
    {
        if (FAILED(m_pEngineCore->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad", VIBufferQuad::Create())))
            return E_FAIL;

        if (FAILED(m_pEngineCore->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Cube", VIBufferCube::Create())))
            return E_FAIL;
    }
    /*Load Prototype Object*/
    {
        if (FAILED(m_pEngineCore->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", BackGround::Create())))
            return E_FAIL;

        if(FAILED(m_pEngineCore->AddPrototype(ENUM_CLASS(LevelID::Static),"Prototype_Object_Socket",Socket::Create())))
            return E_FAIL;

    }
    /*Load Material*/
    {
        if (FAILED(m_pEngineCore->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/background_logo.json",
           "Mtrl_Background_Logo")))
            return E_FAIL;
    }
    return S_OK;
}

void MainApp::AddColliderFilterGroup()
{
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Player), ENUM_CLASS(ColliderFilter::Spawner));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Player), ENUM_CLASS(ColliderFilter::Enemy));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Player), ENUM_CLASS(ColliderFilter::BossPillar));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Player), ENUM_CLASS(ColliderFilter::BossStoneProjectile));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::BossPillar), ENUM_CLASS(ColliderFilter::BossStoneProjectile));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::BossPillar), ENUM_CLASS(ColliderFilter::BossArmProjectile));

    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::PlayerProjectile), ENUM_CLASS(ColliderFilter::Enemy));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::PlayerProjectile), ENUM_CLASS(ColliderFilter::EnemyWeakness));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::PlayerProjectile), ENUM_CLASS(ColliderFilter::EnemyShield));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::PlayerProjectile), ENUM_CLASS(ColliderFilter::BossArm));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::PlayerProjectile), ENUM_CLASS(ColliderFilter::BossStoneProjectile));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::PlayerProjectile), ENUM_CLASS(ColliderFilter::BossPillar));


    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Enemy), ENUM_CLASS(ColliderFilter::Enemy));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Fracture), ENUM_CLASS(ColliderFilter::Fracture));
    m_pEngineCore->AddColliderFilterGroup(ENUM_CLASS(ColliderFilter::Ray), ENUM_CLASS(ColliderFilter::StaticMapObject));
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

    int x = static_cast<_int>(300);
    int y = static_cast<_int>(100);

    windowSize = { 0, 0, WinSizeX, WinSizeY };

    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = windowSize.right - windowSize.left;
    int windowHeight = windowSize.bottom - windowSize.top;

    hWnd = CreateWindowW(
        CLASS_NAME, L"GunFire Reborn",
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
    RECT clipRect{ x + 300,y + 200, x + 600, y + 300 };
    //ClipCursor(&clipRect);

    return true;
}

