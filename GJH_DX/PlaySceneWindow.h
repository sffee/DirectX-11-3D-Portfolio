#pragma once
#include <GJHGameEngineGUI.h>

class PlaySceneWindow : public GJHGUIWindow
{
public:
	static float4 FreeCamPos;
	static float4 FreeCamRot;
	static float4 RenderRot;

public:
	PlaySceneWindow();
	~PlaySceneWindow();

public:
	PlaySceneWindow(const PlaySceneWindow& _Other) = delete;
	PlaySceneWindow(const PlaySceneWindow&& _Other) = delete;

public:
	PlaySceneWindow operator=(const PlaySceneWindow& _Other) = delete;
	PlaySceneWindow operator=(const PlaySceneWindow&& _Other) = delete;

public:
	virtual void OnGUI() override;
};