#pragma once
#include <GJHGameEngineString.h>
#include <map>
#include <GJHGameEngineMemory.h>


class GJHGameEngineScene;
class GJHSceneManager
{
private:
	std::map<GJHGameEngineString, SharedPtr<GJHGameEngineScene>> m_SceneMap;
	SharedPtr<GJHGameEngineScene> m_NextScene;
	SharedPtr<GJHGameEngineScene> m_CurScene;

public:
	SharedPtr<GJHGameEngineScene> GetCurScene()
	{
		return m_CurScene;
	}

public:
	SharedPtr<GJHGameEngineScene> CreateScene(const GJHGameEngineString& _SceneName);
	void ChangeScene(const GJHGameEngineString& _SceneName);

public:
	GJHSceneManager();
	~GJHSceneManager();

public:
	GJHSceneManager(const GJHSceneManager& _Other) = delete;
	GJHSceneManager(const GJHSceneManager&& _Other) = delete;

public:
	GJHSceneManager operator=(const GJHSceneManager& _Other) = delete;
	GJHSceneManager operator=(const GJHSceneManager&& _Other) = delete;

public:
	void Progress();
};