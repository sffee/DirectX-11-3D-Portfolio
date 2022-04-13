#include "Player.h"
#include "define.h"

void Player::Move_Start_Init()
{
	ChangeAnimation(L"Move_Start");
}

void Player::Move_Start_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Move_Loop");
		return;
	}

	if (COMMANDCHECK(L"FDash"))
	{
		State.ChangeState(L"Front_Dash_Start");
		return;
	}

	if (MoveKeyCheck() == false)
	{
		State.ChangeState(L"Move_End_Short");
		return;
	}

	Move();
}

void Player::Move_Loop_Init()
{
	ChangeAnimation(L"Move_Loop");
}

void Player::Move_Loop_State()
{
	if (MoveKeyCheck() == false)
	{
		State.ChangeState(L"Move_End");
		return;
	}

	if (COMMANDCHECK(L"FDash"))
	{
		State.ChangeState(L"Front_Dash_Start");
		return;
	}

	Move();
}

void Player::Move_Stop_Init()
{
	m_SlidingSpeed = 500.f;
	m_SlidingStopSpeed = 500.f;

	ChangeAnimation(L"Move_End");
}

void Player::Move_Stop_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
		return;
	}

	if (COMMANDCHECK(L"FDash"))
	{
		State.ChangeState(L"Front_Dash_Start");
		return;
	}

	if (MoveKeyCheck() == true)
	{
		State.ChangeState(L"Move_Start");
		return;
	}

	MoveStop();
}

void Player::Move_Stop_Short_Init()
{
	ChangeAnimation(L"Move_End_Short");
}

void Player::Move_Stop_Short_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
		return;
	}

	if (COMMANDCHECK(L"FDash"))
	{
		State.ChangeState(L"Front_Dash_Start");
		return;
	}

	if (MoveKeyCheck() == true)
	{
		State.ChangeState(L"Move_Start");
		return;
	}

	MoveStop();
}