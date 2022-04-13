#include <Windows.h>
#include <GJHGameEngineWindow.h>
#include "GJHGameUpdater.h"
#include <GJHGameEngineDebug.h>

#pragma comment(lib, "GameEngineGeometry")
#pragma comment(lib, "GameEngineBase")
#pragma comment(lib, "GameEngineClientBase")
#pragma comment(lib, "GameEngine3D")

int __stdcall wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    GJHGameEngineDebug::LeackCheck();
    //_CrtSetBreakAlloc(988);

    GJHGameEngineWindow::MsgLoop<GJHGameUpdater>();
}