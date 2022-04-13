#pragma once
#include "GJHContentsSceneComponent.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineTime.h>
#include <GJHGameEngineInput.h>
#include "GJHActorTransform.h"

class GJHContentsComponent : public GJHContentsSceneComponent
{
private:
	ActorTransPtr ActorTrans;

public:
	ActorTransPtr GetActorTrans()
	{
		return ActorTrans;
	}

public:
	template<typename T>
	std::shared_ptr<T> FindComponent()
	{
		return GetActor()->FindComponent<T>();
	}

	template<typename T>
	std::shared_ptr<T> CreateComponent()
	{
		return GetActor()->CreateComponent<T>();
	}

	template<typename T, typename ... REST>
	std::shared_ptr<T> CreateComponent(REST ... _Arg)
	{
		return GetActor()->CreateComponent<T>(_Arg...);
	}

public:
	GJHContentsComponent();
	~GJHContentsComponent();

public:
	GJHContentsComponent(const GJHContentsComponent& _Other) = delete;
	GJHContentsComponent(const GJHContentsComponent&& _Other) = delete;

public:
	GJHContentsComponent operator=(const GJHContentsComponent& _Other) = delete;
	GJHContentsComponent operator=(const GJHContentsComponent&& _Other) = delete;

public:
	void Begin() final;
};