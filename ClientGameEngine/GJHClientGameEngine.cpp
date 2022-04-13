#include "GJHClientGameEngine.h"
#include <GJHSceneManager.h>
#include <GJHGameEngineDebug.h>
#include <GJHGameEngineMemory.h>
#include <GJHGameEngineScene.h>
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineActor.h>
#include <GJHDirectConstantBuffer.h>
#include <GJHGameEngineWindow.h>
#include "GJHWorldComponent.h"
#include "GJHActorTransform.h"
#include "GJHRenderManagerComponent.h"
#include "GJHRenderingStruct.h"
#include "GJHGUIComponent.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#pragma comment(lib, "GameEngineClientBase")
#pragma comment(lib, "GameEngineObject")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

__int64 __stdcall GJHClientGameEngine::GuiProc(HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return false;
}

std::shared_ptr<GJHSceneManager> GJHClientGameEngine::m_SceneManager = nullptr;

void GJHClientGameEngine::CreateActorInit(ActorPtr& _Actor)
{
	_Actor->CreateComponent<GJHActorTransform>();
}

std::shared_ptr<GJHGameEngineScene> GJHClientGameEngine::GetCurScene()
{
	return m_SceneManager->GetCurScene();
}

GJHClientGameEngine::GJHClientGameEngine()
{

}

GJHClientGameEngine::~GJHClientGameEngine()
{

}

void GJHClientGameEngine::EngineStart()
{
	m_SceneManager = std::make_shared<GJHSceneManager>();

	GJHGameEngineWindow::CallBackWndProc = &GJHClientGameEngine::GuiProc;

	GJHGameEngineScene::SetActorCreateFunc(CreateActorInit);
}

std::shared_ptr<GJHGameEngineScene> GJHClientGameEngine::CreateScene(const GJHGameEngineString& _Name)
{
	if (m_SceneManager == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_SceneManger == nullptr)");
	}

	std::shared_ptr<GJHGameEngineScene> NewScene = m_SceneManager->CreateScene(_Name);
	NewScene->CreateComponentName<GJHWorldComponent>(L"World");
	NewScene->CreateComponentName<GJHRenderManagerComponent>(L"RenderManager");
	NewScene->CreateComponentName<GJHGUIComponent>(L"GUI");

	return NewScene;
}

void GJHClientGameEngine::ChangeScene(const GJHGameEngineString& _Name)
{
	if (m_SceneManager == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_SceneManager == nullptr)");
	}

	m_SceneManager->ChangeScene(_Name);
}

void GJHClientGameEngine::EngineProgress()
{
	if (m_SceneManager == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_SceneManager == nullptr)");
	}

	m_SceneManager->Progress();
}

void GJHClientGameEngine::EngineEnd()
{
	m_SceneManager = nullptr;
}