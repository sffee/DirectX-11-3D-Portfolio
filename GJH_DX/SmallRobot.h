#pragma once
#include "Monster.h"

class GJHFbxRenderer;
class SmallRobot : public Monster
{
private:
	float m_MoveSpeed;
	float m_RotSpeed;

private:
	std::shared_ptr<GJHFbxRenderer> m_Render;

public:
	SmallRobot();
	~SmallRobot();

public:
	SmallRobot(const SmallRobot& _Other) = delete;
	SmallRobot(const SmallRobot&& _Other) = delete;

public:
	SmallRobot operator=(const SmallRobot& _Other) = delete;
	SmallRobot operator=(const SmallRobot&& _Other) = delete;

public:
	void Start() override;
	void Update() override;

private:
	void TrackingPlayer();
};