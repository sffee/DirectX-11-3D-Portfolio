#include "ObjectPre.h"
#include "GJHSceneManager.h"
#include "GJHGameEngineScene.h"
#include <GJHGameEngineDebug.h>
#include <GJHGameEngineTime.h>

GJHSceneManager::GJHSceneManager()
{

}

GJHSceneManager::~GJHSceneManager()
{

}

SharedPtr<GJHGameEngineScene> GJHSceneManager::CreateScene(const GJHGameEngineString& _SceneName)
{
	if (m_SceneMap.find(_SceneName) != m_SceneMap.end())
	{
		return m_SceneMap[_SceneName];
	}

	m_SceneMap[_SceneName] = SharedPtr<GJHGameEngineScene>(new GJHGameEngineScene());

	return m_SceneMap[_SceneName];
}

void GJHSceneManager::ChangeScene(const GJHGameEngineString& _SceneName)
{
	if (m_SceneMap.find(_SceneName) == m_SceneMap.end())
	{
		GJHGameEngineDebug::AssertMsg("if (m_SceneMap.find(_SceneName) == m_SceneMap.end())");
	}

	m_NextScene = m_SceneMap[_SceneName];
}

void GJHSceneManager::Progress()
{
	if (m_NextScene != nullptr)
	{
		if (m_CurScene != nullptr)
		{
			m_CurScene->SceneChangeEnd();
		}

		m_CurScene = m_NextScene;
		m_NextScene = nullptr;

		if (m_CurScene != nullptr)
		{
			m_CurScene->SceneChangeStart();
		}

		GJHGameEngineTime::GetInst().Reset();
	}

	if (m_CurScene == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_CurScene == nullptr)");
	}

	m_CurScene->UpdatePrev();
	m_CurScene->Update();
	m_CurScene->UpdateNext();
	m_CurScene->RenderPrev();
	m_CurScene->Render();
	m_CurScene->RenderNext();
	m_CurScene->CollisionPrev();
	m_CurScene->Collision();
	m_CurScene->CollisionNext();
	m_CurScene->Release();
}