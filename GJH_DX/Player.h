#pragma once
#include "ObjectBase.h"
#include <GJHCollisionComponent.h>

class GJHFbxAnimationRenderer;
class Player : public ObjectBase
{
private:
	float m_CurMoveSpeed;
	const float m_MoveSpeedLimit;
	const float m_RunSpeedLimit;
	
	float m_SlidingSpeed;
	float m_SlidingStopSpeed;

	float m_CamRotSpeed;
	float m_PlayerRotSpeed;

	std::shared_ptr<GJHActorTransform> m_CamTrans;
	std::shared_ptr<GJHActorTransform> m_CamArm;

private:
	GJHStateUpdater<Player> State;

public:
	void PlayerInit(std::shared_ptr<GJHActorTransform> _CamTrans, float4 _LocalPos, float4 _LocalRot);

public:
	void Start() override;
	void Update() override;

public:
	Player();
	~Player();

public:
	Player(const Player& _Other) = delete;
	Player(const Player&& _Other) = delete;

public:
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(const Player&& _Other) = delete;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

private:
	bool MoveKeyCheck();
	float4 MoveForward();
	void Move();
	void Run();
	void MoveStop();
	void CamRotation();

private:
	void Idle_Init();
	void Idle_State();

	void Move_Start_Init();
	void Move_Start_State();

	void Move_Loop_Init();
	void Move_Loop_State();
	
	void Move_Stop_Init();
	void Move_Stop_State();

	void Move_Stop_Short_Init();
	void Move_Stop_Short_State();

	void Front_Dash_Start_Init();
	void Front_Dash_Start_State();

	void Front_Dash_Init();
	void Front_Dash_State();

	void Front_Dash_Stop_Init();
	void Front_Dash_Stop_State();

	void Run_Start_Init();
	void Run_Start_State();

	void Run_Loop_Init();
	void Run_Loop_State();

	void Run_Stop_Init();
	void Run_Stop_State();

	void Run_Turn_Around_Init();
	void Run_Turn_Around_State();
};