#include "Monster.h"
#include "Player.h"

std::shared_ptr<Player> Monster::Player = nullptr;
std::shared_ptr<GJHActorTransform> Monster::PlayerTrans = nullptr;

Monster::Monster()
{

}

Monster::~Monster()
{

}