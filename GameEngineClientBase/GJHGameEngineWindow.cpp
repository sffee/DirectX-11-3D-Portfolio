#include "GJHGameEngineWindow.h"
#include <assert.h>
#include <GJHGameEngineTime.h>
#include "GJHGameEngineInput.h"
#include <GJHGameEngineDebug.h>

void GJHGameEngineWindowUpdater::PrevDeadTimeUpdate()
{
    GJHGameEngineTime::GetInst().Update();
    GJHGameEngineInput::GetInst().Update();
}

HWND GJHGameEngineWindow::m_WindowHWnd = nullptr;
HDC GJHGameEngineWindow::m_HDC = nullptr;
bool GJHGameEngineWindow::m_bGameOn = true;
float4 GJHGameEngineWindow::m_WindowSize = { 100, 100 };
bool GJHGameEngineWindow::m_IsWinActive = false;

__int64(__stdcall* GJHGameEngineWindow::CallBackWndProc)(HWND,
    unsigned int,
    unsigned __int64,
    __int64);

__int64 __stdcall GJHGameEngineWindow::WndProc(HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
{
    if (CallBackWndProc != nullptr)
    {
        if (CallBackWndProc(hWnd, message, wParam, lParam) != 0)
        {
            return true;
        }
    }

    switch (message)
    {
    case WM_SIZE:
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        m_bGameOn = false;
        break;
    case WM_DPICHANGED:
    {
        break;
    }
    case WM_ACTIVATE:
    {
        if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
        {
            m_IsWinActive = true;
        }
        else if (wParam == WA_INACTIVE)
        {
            m_IsWinActive = false;
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HWND GJHGameEngineWindow::GetWindowHWnd()
{
    return m_WindowHWnd;
}

HDC GJHGameEngineWindow::GetHDC()
{
    return m_HDC;
}

bool GJHGameEngineWindow::GetWinActive()
{
    return m_IsWinActive;
}

void GJHGameEngineWindow::Flash()
{
    FLASHWINFO Info;
    Info.cbSize = sizeof(FLASHWINFO);
    Info.hwnd = m_WindowHWnd;
    Info.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
    Info.uCount = 0;
    Info.dwTimeout = 0;

    FlashWindowEx(&Info);
}

void GJHGameEngineWindow::Start(float4 _Size, const GJHGameEngineString& _String, float4 _StartPos)
{
    {
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &GJHGameEngineWindow::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = nullptr;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"DEFWINDOW";
        wcex.hIconSm = nullptr;
        
        if (0 == RegisterClassExW(&wcex))
        {
            unsigned long ErrorCode = GetLastError();
            assert(false);
        }
    }

    m_WindowHWnd = CreateWindowW(L"DEFWINDOW", _String.c_str(),
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_THICKFRAME |
        WS_MINIMIZEBOX |
        WS_MAXIMIZEBOX,
        _StartPos.uix(), _StartPos.uiy(), CW_USEDEFAULT, 0, nullptr, nullptr, nullptr, nullptr);

    if (nullptr == m_WindowHWnd)
    {
        assert(false);
    }

    m_HDC = GetDC(m_WindowHWnd);

    ShowWindow(m_WindowHWnd, SW_SHOW);
    UpdateWindow(m_WindowHWnd);

    SetSize(_Size);
}

float4 GJHGameEngineWindow::GetSize()
{
    return m_WindowSize;
}

void GJHGameEngineWindow::SetSize(const float4& _Size)
{
    m_WindowSize = _Size;

    int X = m_WindowSize.ix();
    int Y = m_WindowSize.iy();

    RECT Rc = { 0, 0, m_WindowSize.ix(), m_WindowSize.iy() };
    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);
    SetWindowPos(m_WindowHWnd, nullptr,
        0, 0,
        Rc.right - Rc.left,
        Rc.bottom - Rc.top,
        SWP_NOMOVE | SWP_NOZORDER);
}

void GJHGameEngineWindow::End()
{
    ReleaseDC(m_WindowHWnd, m_HDC);
    DestroyWindow(m_WindowHWnd);
    m_HDC = nullptr;
    m_WindowHWnd = nullptr;
}


float4 GJHGameEngineWindow::GetMousePos()
{
    POINT MousePos;

    GetCursorPos(&MousePos);

    ScreenToClient(m_WindowHWnd, &MousePos);

    return float4((float)MousePos.x, (float)MousePos.y);
}