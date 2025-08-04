#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "Application.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Application* Game = Application::Create(hInstance, nCmdShow);

    if (!Game)
    {
        MSG_BOX("Create Failed : Application");
        return -1;
    }
    
    Game->Run();

    _uint refCnt = Safe_Release(Game);
    if (refCnt)
    {
        MSG_BOX("Release Failed : Application");
        return -1;
    }

    return 0;
}
