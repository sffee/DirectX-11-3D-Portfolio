#pragma once
#include <GJHContentsSceneComponent.h>
#include <GJHFreeCamera.h>

class PlaySceneWindow;
class Player;
class PlayScene : public GJHContentsSceneComponent
{
private:
	std::shared_ptr<GJHFreeCamera> m_FreeCam;
	std::shared_ptr<GJHCamera> m_CollisionDebugCam;
	std::shared_ptr<PlaySceneWindow> m_PlaySceneWindow;
	std::shared_ptr<Player> m_Player;

public:
	PlayScene();
	~PlayScene();

public:
	PlayScene(const PlayScene& _Other) = delete;
	PlayScene(const PlayScene&& _Other) = delete;

public:
	PlayScene operator=(const PlayScene& _Other) = delete;
	PlayScene operator=(const PlayScene&& _Other) = delete;

public:
	void Start() override;
	void Update() override;

private:
	void CreateCam();
	void CreateLight();
	void CreateMap();
	void CreatePlayer();
	void CreateMonster();
};