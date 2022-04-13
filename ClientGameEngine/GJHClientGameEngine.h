#pragma once
#include <BaseMinimalHeader.h>
#include <GJHGameEngineScene.h>

class GJHSceneManager;
class GJHRenderManagerComponent;
class GJHWorldComponent;
class GJHClientGameEngine
{
public:
	static void CreateActorInit(ActorPtr& _Actor);

private:
	static std::shared_ptr<GJHSceneManager> m_SceneManager;

	static __int64 __stdcall GuiProc(HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);

public:
	static std::shared_ptr<GJHGameEngineScene> GetCurScene();

public:
	static void EngineStart();
	static void EngineProgress();
	static void EngineEnd();
	static void Release();

public:
	static std::shared_ptr<GJHGameEngineScene> CreateScene(const GJHGameEngineString& _Name);
	static void ChangeScene(const GJHGameEngineString& _Name);

	template<typename StartComType>
	static void CreateScene(const GJHGameEngineString& _Name)
	{
		std::shared_ptr<GJHGameEngineScene> NewScene = CreateScene(_Name);
		NewScene->CreateComponent<StartComType>();
	}

public:
	GJHClientGameEngine();
	~GJHClientGameEngine();

public:
	GJHClientGameEngine(const GJHClientGameEngine& _Other) = delete;
	GJHClientGameEngine(const GJHClientGameEngine&& _Other) = delete;

public:
	GJHClientGameEngine operator=(const GJHClientGameEngine& _Other) = delete;
	GJHClientGameEngine operator=(const GJHClientGameEngine&& _Other) = delete;
};