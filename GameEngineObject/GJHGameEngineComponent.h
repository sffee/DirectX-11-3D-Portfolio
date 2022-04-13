#pragma once
#include "GJHGameUpdaterInterface.h"
#include <GJHUpdaterBase.h>
#include <GJHNameBase.h>
#include <GJHTypeBase.h>

class GJHGameEngineActor;
class GJHGameEngineScene;
class GJHComponentManagement;
class GJHGameEngineComponent : public GJHGameUpdaterInterface, public GJHUpdaterBase, public GJHNameBase, public GJHTypeBase, public std::enable_shared_from_this<GJHGameEngineComponent>
{
private:
	friend GJHComponentManagement;

private:
	GJHGameEngineActor* m_Actor;
	GJHGameEngineScene* m_Scene;

private:
	void SetActor(GJHGameEngineActor* _Actor)
	{
		m_Actor = _Actor;
	}

	void SetScene(GJHGameEngineScene* _Scene)
	{
		m_Scene = _Scene;
	}

public:
	GJHGameEngineActor* GetActor()
	{
		return m_Actor;
	}

	GJHGameEngineScene* GetScene()
	{
		return m_Scene;
	}

public:
	GJHGameEngineComponent();
	~GJHGameEngineComponent();

public:
	GJHGameEngineComponent(const GJHGameEngineComponent& _Other) = delete;
	GJHGameEngineComponent(const GJHGameEngineComponent&& _Other) = delete;

public:
	GJHGameEngineComponent operator=(const GJHGameEngineComponent& _Other) = delete;
	GJHGameEngineComponent operator=(const GJHGameEngineComponent&& _Other) = delete;

public:
	void SceneChangeStart() override { }
	void SceneChangeEnd() override { }

	void UpdatePrev() override { }
	void Update() override { }
	void UpdateNext() override { }
	void RenderPrev() override { }
	void Render() override { }
	void RenderNext() override { }
	void CollisionPrev() override { }
	void Collision() override { }
	void CollisionNext() override { }
	void Release() override { }
};