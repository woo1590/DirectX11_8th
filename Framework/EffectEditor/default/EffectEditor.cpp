// EffectEditor.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "EffectEditor.h"
#include "MainApp.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    MainApp* Game = MainApp::Create(hInstance, nCmdShow);

    if (!Game)
    {
        MSG_BOX("Create Failed : MainApp");
        return -1;
    }

    Game->Run();

    _uint refCnt = Safe_Release(Game);
    if (refCnt)
    {
        MSG_BOX("Release Failed : MainApp");
        return -1;
    }

    return 0;
}
