#include "Player.h"
#include "define.h"

void Player::Idle_Init()
{
	ChangeAnimation(L"Idle");
}

void Player::Idle_State()
{
	if (MoveKeyCheck() == true)
	{
		State.ChangeState(L"Move_Start");
		return;
	}
}