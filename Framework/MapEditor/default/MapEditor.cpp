#include "MapEditorPCH.h"
#include "framework.h"
#include "MapEditor.h"
#include "MainApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    MainApp* Editor = MainApp::Create(hInstance, nCmdShow);

    if (!Editor)
    {
        MSG_BOX("Create Failed : MainApp");
        return -1;
    }

    Editor->Run();

    _uint refCnt = Safe_Release(Editor);
    if (refCnt)
    {
        MSG_BOX("Release Failed : MainApp");
        return -1;
    }

    return 0;
}
