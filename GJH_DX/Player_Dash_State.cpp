#include "Player.h"
#include "define.h"

void Player::Front_Dash_Start_Init()
{
	ChangeAnimation(L"Front_Dash_Start");
}

void Player::Front_Dash_Start_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Front_Dash");
		return;
	}

	if (MoveKeyCheck() == true)
	{
		State.ChangeState(L"Front_Dash_Stop");
		return;
	}

	Run();
}

void Player::Front_Dash_Init()
{
	ChangeAnimation(L"Front_Dash");
}

void Player::Front_Dash_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
		return;
	}

	if (MoveKeyCheck() == true)
	{
		State.ChangeState(L"Front_Dash_Stop");
		return;
	}

	Run();
}
	 
void Player::Front_Dash_Stop_Init()
{
	ChangeAnimation(L"Front_Dash_Stop");
}

void Player::Front_Dash_Stop_State()
{
	if (AnimationEnd())
	{
		State.ChangeState(L"Idle");
		return;
	}

	if (MoveKeyCheck() == true)
	{
		State.ChangeState(L"Move_Start");
		return;
	}

	MoveStop();
}