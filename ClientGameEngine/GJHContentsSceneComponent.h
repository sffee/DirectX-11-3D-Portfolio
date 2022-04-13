#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineActor.h>
#include "GJHActorTransform.h"

class GJHGUIComponent;
class GJHRenderManagerComponent;
class GJHWorldComponent;
class GJHSpriteRenderer;
class GJHDirectTexture2D;
class GJHContentsSceneComponent : public GJHGameEngineComponent
{
private:
	static float4 m_CamSize;
	static float m_StageTime;

public:
	static void SetCamSize(const float4& _CamSize)
	{
		m_CamSize = _CamSize;
	}

	static float GetStageTime()
	{
		return m_StageTime;
	}

	static void AddStageTime(const float _Value)
	{
		m_StageTime += _Value;
	}

	static void ResetStageTime()
	{
		m_StageTime = 0.f;
	}

public:
	enum class ACTORTYPE
	{
		NONE,
		PLAYER,
		MONSTER,
		BULLET,
		ITEM,
		EFFECT,
		BACKGROUND,
		DOOR,
		GATE,
		UI
	};

private:
	std::shared_ptr<GJHWorldComponent> World;
	std::shared_ptr<GJHGUIComponent> GUI;

public:
	std::shared_ptr<GJHGUIComponent> GetGUI()
	{
		return GUI;
	}

	std::shared_ptr<GJHWorldComponent> GetWorld()
	{
		return World;
	}

public:
	GJHContentsSceneComponent();
	~GJHContentsSceneComponent();

public:
	GJHContentsSceneComponent(const GJHContentsSceneComponent& _Other) = delete;
	GJHContentsSceneComponent(const GJHContentsSceneComponent&& _Other) = delete;

public:
	GJHContentsSceneComponent operator=(const GJHContentsSceneComponent& _Other) = delete;
	GJHContentsSceneComponent operator=(const GJHContentsSceneComponent&& _Other) = delete;

public:
	void Begin() override;
};