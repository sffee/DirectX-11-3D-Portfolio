#pragma once
#include <Windows.h>
#include <GJHGameEngineMath.h>
#include <GJHGameEngineString.h>
#include <GJHGameEngineTime.h>
#include <functional>
#include <GJHGameEngineSound.h>

class GJHGameEngineWindowUpdater
{
public:
	virtual void UpdateStart() = 0;
	virtual void DeadTimeUpdate() = 0;
	virtual void End() = 0;

public:
	virtual void PrevDeadTimeUpdate();

protected:
	GJHGameEngineWindowUpdater() {}
	~GJHGameEngineWindowUpdater() {}
};

class GJHGameEngineWindow
{
private:
	static bool m_bGameOn;
	static float4 m_WindowSize;

	static HWND m_WindowHWnd;
	static HDC m_HDC;

	static bool m_IsWinActive;

public:
	static HWND GetWindowHWnd();
	static HDC GetHDC();
	static bool GetWinActive();
	
public:
	static void Flash();

public:
	static void Start(float4 _Size, const GJHGameEngineString& _Name, float4 _StartPos = float4::ZERO);
	static void End();

public:
	static void SetSize(const float4& _Size);
	static float4 GetSize();

	template<typename T>
	static void MsgLoop()
	{
		GJHGameEngineSound::Init();

		MSG Msg;

		T NewT = T();
		NewT.UpdateStart();

		while (m_bGameOn == true)
		{
			if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
			{
				NewT.PrevDeadTimeUpdate();
				NewT.DeadTimeUpdate();

				::TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			else
			{
				NewT.PrevDeadTimeUpdate();
				NewT.DeadTimeUpdate();
			}
		}

		NewT.End();
	}

private:
	static __int64 WndProc(HWND hWnd,
		unsigned int message,
		unsigned __int64 wParam,
		__int64 lParam);

public:
	static __int64(__stdcall* CallBackWndProc)(HWND,
		unsigned int,
		unsigned __int64,
		__int64);

public:
	static float4 GetMousePos();

private:
	GJHGameEngineWindow() {}
	virtual ~GJHGameEngineWindow() = 0 {}
};