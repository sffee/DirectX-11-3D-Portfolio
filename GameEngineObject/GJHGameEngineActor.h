#pragma once
#include "GJHComponentManagement.h"
#include "GJHGameUpdaterInterface.h"
#include <GJHUpdaterBase.h>

class GJHGameEngineScene;
class GJHGameEngineActor;
class GJHGameEngineActor : public GJHComponentManagement
{
private:
	friend GJHGameEngineScene;

private:
	GJHGameEngineScene* m_Scene;
	GJHGameEngineString m_ClassName;

private:
	void SetScene(GJHGameEngineScene* _Scene)
	{
		m_Scene = _Scene;
	}

public:
	GJHGameEngineScene* GetScene()
	{
		return m_Scene;
	}

	void SetClassName(const GJHGameEngineString& _Name)
	{
		m_ClassName = _Name;
	}

public:
	GJHGameEngineActor();
	~GJHGameEngineActor();

public:
	GJHGameEngineActor(const GJHGameEngineActor& _Other) = delete;
	GJHGameEngineActor(const GJHGameEngineActor&& _Other) = delete;

public:
	GJHGameEngineActor operator=(const GJHGameEngineActor& _Other) = delete;
	GJHGameEngineActor operator=(const GJHGameEngineActor&& _Other) = delete;
};