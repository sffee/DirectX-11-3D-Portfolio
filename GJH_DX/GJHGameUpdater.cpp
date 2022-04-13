#include "GJHGameUpdater.h"
#include <GJHClientGameEngine.h>
#include "TitleScene.h"
#include <GJHGameEngineGUI.h>
#include "GJHGlobalAppWindow.h"
#include <GJHContentsSceneComponent.h>
#include "define.h"
#include <GJHGameEngineDirectory.h>
#include <GJHDirectTexture2D.h>
#include <GJHDirectSprite.h>
#include "RenderingWindow.h"
#include <GJHGameEngineSound.h>
#include <GJHGameEngineThread.h>
#include <GJHDirectFbx.h>
#include <GJHDirectFont.h>
#include "LoadingScene.h"

#pragma comment(lib, "ClientGameEngine")

bool GJHGameUpdater::IsLoadingEnd = false;

void GJHGameUpdater::ResLodingThread()
{
	SetThreadDescription(GetCurrentThread(), L"ResLodingThread");

	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"GJH");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Texture");
		auto Arr = NewDir.AllFile(L"*.*");
		for (size_t i = 0; i < Arr.size(); i++)
		{
			GJHDirectTexture2D::Load(Arr[i].FullPath());
		}
	}

	IsLoadingEnd = true;
}

void GJHGameUpdater::MakeKey()
{
	CREATECOMMAND("FDash");
	ADDCOMMANDKEY("FDash", 0.5f, 'W');
	ADDCOMMANDKEY("FDash", 0.5f, 'W');

	CREATEKEY(L"A", 'A');
	CREATEKEY(L"D", 'D');
	CREATEKEY(L"W", 'W');
	CREATEKEY(L"S", 'S');

	CREATEKEY(L"L", 'A');
	CREATEKEY(L"R", 'D');
	CREATEKEY(L"F", 'W');
	CREATEKEY(L"B", 'S');
	CREATEKEY(L"LF", 'A', 'W');
	CREATEKEY(L"RF", 'D', 'W');
	CREATEKEY(L"LB", 'A', 'S');
	CREATEKEY(L"RB", 'D', 'S');

	CREATEKEY(L"Q", 'Q');
	CREATEKEY(L"E", 'E');
	CREATEKEY(L"O", 'O');

	CREATEKEY(L"MouseClick", VK_LBUTTON);
	CREATEKEY(L"MouseRClick", VK_RBUTTON);
	CREATEKEY(L"LClick", VK_LBUTTON);
	CREATEKEY(L"RClick", VK_RBUTTON);
	CREATEKEY(L"LShift", VK_LSHIFT);
	CREATEKEY(L"F1", VK_F1);
	CREATEKEY(L"F2", VK_F2);
	CREATEKEY(L"F3", VK_F3);
	CREATEKEY(L"F4", VK_F4);
	CREATEKEY(L"F5", VK_F5);
	CREATEKEY(L"F6", VK_F6);
	CREATEKEY(L"F7", VK_F7);
	CREATEKEY(L"F8", VK_F8);
	CREATEKEY(L"F9", VK_F9);
	CREATEKEY(L"F10", VK_F10);
	CREATEKEY(L"F11", VK_F11);
	CREATEKEY(L"F12", VK_F12);
	CREATEKEY(L"Numpad0", VK_NUMPAD0);
	CREATEKEY(L"Numpad1", VK_NUMPAD1);
	CREATEKEY(L"Numpad2", VK_NUMPAD2);
	CREATEKEY(L"Numpad3", VK_NUMPAD3);
	CREATEKEY(L"Numpad4", VK_NUMPAD4);
	CREATEKEY(L"Numpad5", VK_NUMPAD5);
	CREATEKEY(L"Numpad6", VK_NUMPAD6);
	CREATEKEY(L"Numpad7", VK_NUMPAD7);
	CREATEKEY(L"Numpad8", VK_NUMPAD8);
	CREATEKEY(L"Numpad9", VK_NUMPAD9);
	CREATEKEY(L"NumpadMul", VK_MULTIPLY);
	CREATEKEY(L"Enter", VK_RETURN);
	CREATEKEY(L"Space", VK_SPACE);
	CREATEKEY(L"Home", VK_HOME);
}

void GJHGameUpdater::UpdateStart()
{
	GJHGameEngineWindow::Start(float4{ 1920, 1080 }, GJHGameEngineString(L"Main"), { 300, 150 });
	GJHGameEngineDevice::Init(GJHGameEngineWindow::GetWindowHWnd(), GJHGameEngineWindow::GetSize());
	GJHGameEngineGUI::Init();

	GJHClientGameEngine::EngineStart();

	GJHContentsSceneComponent::SetCamSize(float4(CAMSIZEX, CAMSIZEY));

	MakeKey();

	GJHGameEngineThread::CreateThread(L"LodingThread", &GJHGameUpdater::ResLodingThread, this);

	GJHClientGameEngine::CreateScene<LoadingScene>(L"LoadingScene");
	GJHClientGameEngine::ChangeScene(L"LoadingScene");
}

void GJHGameUpdater::DeadTimeUpdate()
{
	GJHGameEngineSound::Update();
	GJHClientGameEngine::EngineProgress();
}

void GJHGameUpdater::End()
{
	GJHClientGameEngine::EngineEnd();
	GJHGameEngineSound::Release();
	GJHGameEngineGUI::Release();
	GJHDirectFbx::Release();
	GJHGameEngineDevice::Release();
	GJHGameEngineWindow::End();
}