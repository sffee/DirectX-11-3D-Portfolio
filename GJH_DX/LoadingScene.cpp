#include "LoadingScene.h"
#include "GJHGlobalAppWindow.h"
#include "RenderingWindow.h"
#include "FBXWindow.h"
#include <GJHGameEngineGUI.h>
#include <GJHDirectSprite.h>
#include <GJHClientGameEngine.h>
#include <GJHGameEngineThread.h>
#include "GJHGameUpdater.h"
#include "TitleScene.h"
#include "PlayScene.h"

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Update()
{
	if (GJHGameUpdater::IsLoadingEnd == true)
	{
		std::shared_ptr<GJHGlobalAppWindow> GWindow = GJHGameEngineGUI::CreateGUIWindow<GJHGlobalAppWindow>(L"EngineStatus");
		GJHGameEngineGUI::CreateGUIWindow<RenderingWindow>(L"RenderingStatus");
		GJHGameEngineGUI::CreateGUIWindow<FBXWindow>(L"FBXWindow");

		//GWindow->SceneName.push_back(L"TitleScene");
		GWindow->SceneName.push_back(L"PlayScene");

		GJHClientGameEngine::CreateScene<TitleScene>(L"TitleScene");
		GJHClientGameEngine::CreateScene<PlayScene>(L"PlayScene");
		GJHClientGameEngine::ChangeScene(L"PlayScene");
	}

}

void LoadingScene::Start()
{
}