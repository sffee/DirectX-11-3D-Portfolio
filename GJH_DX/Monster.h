#pragma once
#include <GJHContentsComponent.h>
#include "ObjectBase.h"

class Player;
class Monster : public ObjectBase
{
public:
	static std::shared_ptr<Player> Player;
	static std::shared_ptr<GJHActorTransform> PlayerTrans;

public:
	Monster();
	~Monster();

public:
	Monster(const Monster& _Other) = delete;
	Monster(const Monster&& _Other) = delete;

public:
	Monster operator=(const Monster& _Other) = delete;
	Monster operator=(const Monster&& _Other) = delete;
};