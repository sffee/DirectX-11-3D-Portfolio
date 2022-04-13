#pragma once
#include <GJHContentsSceneComponent.h>

class LoadingScene : public GJHContentsSceneComponent
{
public:
	LoadingScene();
	~LoadingScene();

public:
	LoadingScene(const LoadingScene& _Other) = delete;
	LoadingScene(const LoadingScene&& _Other) = delete;

public:
	LoadingScene& operator=(const LoadingScene& _Other) = delete;
	LoadingScene& operator=(const LoadingScene&& _Other) = delete;

public:
	void Start() override;
	void Update() override;
};

