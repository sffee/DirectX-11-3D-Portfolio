#pragma once
#include <list>
#include <GJHGameEngineMemory.h>
#include "GJHComponentManagement.h"
#include "GJHGameUpdaterInterface.h"

typedef std::shared_ptr<GJHGameEngineActor> ActorPtr;

class GJHGameEngineActor;
class GJHGameEngineScene : public GJHComponentManagement
{
private:
	static void(__stdcall* m_ActorCreateFunc)(ActorPtr& _Actor);

public:
	static void SetActorCreateFunc(void(__stdcall* _ActorCreateFunc)(ActorPtr&))
	{
		m_ActorCreateFunc = _ActorCreateFunc;
	}

private:
	std::list<ActorPtr> m_Actors;

public:
	std::list<ActorPtr>& GetActorList();

public:
	GJHGameEngineScene();
	~GJHGameEngineScene();

public:
	GJHGameEngineScene(const GJHGameEngineScene& _Other) = delete;
	GJHGameEngineScene(const GJHGameEngineScene&& _Other) = delete;

public:
	GJHGameEngineScene operator=(const GJHGameEngineScene& _Other) = delete;
	GJHGameEngineScene operator=(const GJHGameEngineScene&& _Other) = delete;

public:
	ActorPtr CreateActor();

public:
	void SceneChangeStart() override;
	void SceneChangeEnd() override;

	void UpdatePrev() override;
	void Update() override;
	void UpdateNext() override;
	void CollisionPrev() override;
	void Collision() override;
	void CollisionNext() override;
	void RenderPrev() override;
	void Render() override;
	void DebugRender() override;
	void RenderNext() override;
	void Release() override;
	void OnGUI() override;
};