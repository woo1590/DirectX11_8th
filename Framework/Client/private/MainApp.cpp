#include "pch.h"
#include "MainApp.h"

MainApp::MainApp()
{
}

MainApp* MainApp::Create()
{
    MainApp* Instance = new MainApp();

    if (FAILED(Instance->Initialize()))
    {
        Instance = nullptr;
    }

    return Instance;
}

HRESULT MainApp::Initialize()
{
    return S_OK;
}

void MainApp::Update(_float dt)
{
    //main game loop
}

void MainApp::Free()
{

}
