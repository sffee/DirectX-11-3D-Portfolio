#include "ColTestActor.h"
#include "define.h"

ColTestActor::ColTestActor()
{

}

ColTestActor::~ColTestActor()
{

}

void ColTestActor::Start()
{
	CreateCollision((int)CollisionType::Monster, COLTYPE::OBB, &ColTestActor::ColEnter, &ColTestActor::ColStay, &ColTestActor::ColExit, *this);
}

void ColTestActor::Update()
{

}

void ColTestActor::SetCollisionScale(const float4& _Scale)
{
	m_Collision->SetLocalScale(_Scale);
}

void ColTestActor::SetCollisionRotation(const float4& _Rotation)
{
	m_Collision->SetLocalRotation(_Rotation);
}

void ColTestActor::ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	int a = 0;
}

void ColTestActor::ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	int a = 0;
}

void ColTestActor::ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other)
{
	int a = 0;
}

