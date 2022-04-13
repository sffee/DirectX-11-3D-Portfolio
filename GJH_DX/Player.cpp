#include "Player.h"
#include "Monster.h"
#include "define.h"
#include <GJHGameEngineScene.h>
#include <GJHGameEngineInput.h>
#include <GJHFreeCamera.h>
#include "PlaySceneWindow.h"
#include <GJHFbxAnimationRenderer.h>
#include <GJHCollisionComponent.h>
#include <GJHWorldComponent.h>

Player::Player() :
	m_MoveSpeedLimit(500.f),
	m_RunSpeedLimit(1000.f),
	m_CamRotSpeed(30.0f),
	m_PlayerRotSpeed(0.5f)
{
}

Player::~Player()
{
}

void Player::Start()
{
	Monster::Player = FindComponent<Player>();
	Monster::PlayerTrans = GetActorTrans();

	CreateCollision((int)CollisionType::Player, COLTYPE::OBB, &Player::ColEnter, &Player::ColStay, &Player::ColExit, *this);
	m_Collision->SetLocalScale({ 40.f, 170.f, 30.f });
	m_Collision->SetLocalPosition({ 0.f, 87.f, 0.f });

	m_AniRender = FindComponent<GJHFbxAnimationRenderer>();
	CreateAnimation(L"Idle", L"Player.USERMESH", L"Player_Idle.USERANI", 0, true, 1.f / 40.f);
	CreateAnimation(L"Move_Start", L"Player.USERMESH", L"Player_Move_Start.USERANI", 0, false, 1.f / 40.f);
	CreateAnimation(L"Move_Loop", L"Player.USERMESH", L"Player_Move_Loop.USERANI", 0, true, 1.f / 40.f);
	CreateAnimation(L"Move_End", L"Player.USERMESH", L"Player_Move_Stop.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Move_End_Short", L"Player.USERMESH", L"Player_Move_Stop_Short.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Front_Dash_Start", L"Player.USERMESH", L"Player_Front_Dash_Start.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Front_Dash", L"Player.USERMESH", L"Player_Front_Dash.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Front_Dash_Stop", L"Player.USERMESH", L"Player_Front_Dash_Stop.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Run_Start", L"Player.USERMESH", L"Player_Run_Start.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Run_Loop", L"Player.USERMESH", L"Player_Run_Loop.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Run_Stop", L"Player.USERMESH", L"Player_Run_Stop.USERANI", 0, false, 1.f / 50.f);
	CreateAnimation(L"Run_Turn_Around", L"Player.USERMESH", L"Player_Run_Turn_Around.USERANI", 0, false, 1.f / 50.f);

	ChangeAnimation(L"Idle");

	State.CreateState(L"Idle", &Player::Idle_Init, &Player::Idle_State, nullptr, this);
	State.CreateState(L"Move_Start", &Player::Move_Start_Init, &Player::Move_Start_State, nullptr, this);
	State.CreateState(L"Move_Loop", &Player::Move_Loop_Init, &Player::Move_Loop_State, nullptr, this);
	State.CreateState(L"Move_End", &Player::Move_Stop_Init, &Player::Move_Stop_State, nullptr, this);
	State.CreateState(L"Move_End_Short", &Player::Move_Stop_Short_Init, &Player::Move_Stop_Short_State, nullptr, this);
	State.CreateState(L"Front_Dash_Start", &Player::Front_Dash_Start_Init, &Player::Front_Dash_Start_State, nullptr, this);
	State.CreateState(L"Front_Dash", &Player::Front_Dash_Init, &Player::Front_Dash_State, nullptr, this);
	State.CreateState(L"Front_Dash_Stop", &Player::Front_Dash_Stop_Init, &Player::Front_Dash_Stop_State, nullptr, this);
	State.CreateState(L"Run_Start", &Player::Run_Start_Init, &Player::Run_Start_State, nullptr, this);
	State.CreateState(L"Run_Loop", &Player::Run_Loop_Init, &Player::Run_Loop_State, nullptr, this);
	State.CreateState(L"Run_Stop", &Player::Run_Stop_Init, &Player::Run_Stop_State, nullptr, this);
	State.CreateState(L"Run_Turn_Around", &Player::Run_Turn_Around_Init, &Player::Run_Turn_Around_State, nullptr, this);

	State.ChangeState(L"Idle");

	m_CurMoveSpeed = 0.f;

	m_AniRender->SetLocalScale({ 100.f, 100.f, 100.f });
}	


void Player::PlayerInit(std::shared_ptr<GJHActorTransform> _CamTrans, float4 _LocalPos, float4 _LocalRot)
{
	std::shared_ptr<GJHGameEngineActor> Actor = GetScene()->CreateActor();
	m_CamArm = Actor->FindComponent<GJHActorTransform>();
	m_CamArm->AttachParent(GetActorTrans());

	m_CamTrans = _CamTrans;
	m_CamTrans->AttachParent(m_CamArm);
	m_CamTrans->SetLocalPosition(_LocalPos);
	m_CamTrans->SetLocalRotation(_LocalRot); 
}

void Player::Update()
{
	if (m_CamTrans == nullptr)
	{
		return;
	}

	if (KEYDOWN(L"F1"))
	{
		GJHGameEngineInput::SetMouseMoveToCenter(true);
	}
	else if (KEYUP(L"F1"))
	{
		GJHGameEngineInput::SetMouseMoveToCenter(false);
	}

	if (KEYPRESS(L"Q"))
	{
		m_CamArm->SetLocalRotation(float4(0.f, 0.f, 0.f));

		return;
		std::shared_ptr<GJHFreeCamera> FreeCam = m_CamTrans->GetActor()->FindComponent<GJHFreeCamera>();

		if (FreeCam == nullptr)
		{
			return;
		}

		FreeCam->FreeCamModeOn();

		m_CamTrans->DetachParent();
		m_CamTrans = nullptr;
	}

	CamRotation();

	State.Update();
	ObjectBase::Update();

	PlaySceneWindow::RenderRot = m_AniRender->GetWorldRotation();
}

void Player::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	int a = 0;
}

void Player::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	
}

void Player::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{

}

void Player::CamRotation()
{
	float4 MouseDistance = GJHGameEngineInput::GetMouse3DDistance();
	float4 CamRotEuler = float4(-MouseDistance.y * DELTATIME(m_CamRotSpeed), MouseDistance.x * DELTATIME(m_CamRotSpeed), 0.f);

	float4 CamArmRotEuler = m_CamArm->GetLocalRotation() + CamRotEuler;

	if (70.f <= CamArmRotEuler.x)
	{
		m_CamArm->SetLocalRotation(float4(70.f, CamArmRotEuler.y, 0.f));
		m_CamTrans->SetLocalRotation(float4(30.f, 0.f, 0.f));
		return;
	}

	if (CamArmRotEuler.x <= -70.f)
	{
		m_CamArm->SetLocalRotation(float4(-70.f, CamArmRotEuler.y, 0.f));
		return;
	}

	m_CamArm->AddLocalRotation(CamRotEuler);

	if (0.f <= m_CamArm->GetLocalRotation().x)
	{
		m_CamTrans->SetLocalRotation(float4(m_CamArm->GetLocalRotation().x / 2.3333f, 0.f, 0.f));
	}
	else
	{
		m_CamTrans->SetLocalRotation(float4::ZERO);
	}
}

bool Player::MoveKeyCheck()
{
	if (KEYPRESS(L"LF")
		|| KEYPRESS(L"RF")
		|| KEYPRESS(L"LB")
		|| KEYPRESS(L"RB")
		|| KEYPRESS(L"L")
		|| KEYPRESS(L"R")
		|| KEYPRESS(L"F")
		|| KEYPRESS(L"B"))
	{
		return true;
	}

	return false;
}

float4 Player::MoveForward()
{
	float4 CamForward = m_CamArm->GetWorldForward();
	CamForward.y = 0.0f;
	CamForward = CamForward.GetNormalize();

	float4 InputForward = CamForward;

	if (KEYPRESS(L"LF"))
	{
		InputForward.RotationYDeg(-45.f);
	}
	else if (KEYPRESS(L"RF"))
	{
		InputForward.RotationYDeg(45.f);
	}
	else if (KEYPRESS(L"LB"))
	{
		InputForward.RotationYDeg(-135.f);
	}
	else if (KEYPRESS(L"RB"))
	{
		InputForward.RotationYDeg(135.f);
	}
	else if (KEYPRESS(L"L"))
	{
		InputForward.RotationYDeg(-90.f);
	}
	else if (KEYPRESS(L"R"))
	{
		InputForward.RotationYDeg(90.f);
	}
	else if (KEYPRESS(L"B"))
	{
		InputForward.RotationYDeg(180.f);
	}

	return InputForward;
}

void Player::Move()
{	
	float4 InputForward = MoveForward();
	float4 MeshForward = m_AniRender->GetWorldForward();
	float RenderAngle = m_AniRender->GetWorldRotation().y;

	float MoveRot = RenderAngle - float4::CalVectorAngleDegY360(MeshForward, InputForward);
	float Angle = float4::Lerp(m_AniRender->GetWorldRotation().y, MoveRot, m_PlayerRotSpeed);
	if (isnan(Angle) == false)
	{
		m_AniRender->SetLocalRotationY(Angle);
	}

	m_CurMoveSpeed += DELTATIME(2000.f);
	if (m_MoveSpeedLimit <= m_CurMoveSpeed)
	{
		m_CurMoveSpeed = m_MoveSpeedLimit;
	}

	std::shared_ptr<GJHWorldComponent> WorldCom = GetScene()->FindComponent<GJHWorldComponent>();

	auto Collist = WorldCom->GetCollisionCheck(m_Collision, InputForward * DELTATIME(m_CurMoveSpeed), (int)CollisionType::Monster);
	if (Collist.empty())
	{
		GetActorTrans()->SetLocalMove(InputForward * DELTATIME(m_CurMoveSpeed));
	}
}

void Player::MoveStop()
{
	m_CurMoveSpeed -= DELTATIME(2000.f);
	if (m_CurMoveSpeed <= 0.f)
	{
		m_CurMoveSpeed = 0.f;
	}

	GetActorTrans()->SetLocalMove(m_AniRender->GetWorldForward() * DELTATIME(m_CurMoveSpeed));
}

void Player::Run()
{
	float4 InputForward = MoveForward();
	float4 MeshForward = m_AniRender->GetWorldForward();
	float RenderAngle = m_AniRender->GetWorldRotation().y;

	float MoveRot = RenderAngle - float4::CalVectorAngleDegY360(MeshForward, InputForward);
	float Angle = float4::Lerp(m_AniRender->GetWorldRotation().y, MoveRot, m_PlayerRotSpeed);
	if (isnan(Angle) == false)
	{
		m_AniRender->SetLocalRotationY(Angle);
	}

	m_CurMoveSpeed += DELTATIME(6000.f);
	if (m_RunSpeedLimit <= m_CurMoveSpeed)
	{
		m_CurMoveSpeed = m_RunSpeedLimit;
	}

	std::shared_ptr<GJHWorldComponent> WorldCom = GetScene()->FindComponent<GJHWorldComponent>();

	auto Collist = WorldCom->GetCollisionCheck(m_Collision, InputForward * DELTATIME(m_CurMoveSpeed), (int)CollisionType::Monster);
	if (Collist.empty())
	{
		GetActorTrans()->SetLocalMove(InputForward * DELTATIME(m_CurMoveSpeed));
	}
}