#pragma once
#include "ObjectBase.h"

class ColTestActor : public ObjectBase
{
public:
	ColTestActor();
	~ColTestActor();

public:
	ColTestActor(const ColTestActor& _Other) = delete;
	ColTestActor(const ColTestActor&& _Other) = delete;

public:
	ColTestActor operator=(const ColTestActor& _Other) = delete;
	ColTestActor operator=(const ColTestActor&& _Other) = delete;

private:
	void ColEnter(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColStay(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);
	void ColExit(GJHCollisionComponent* _this, GJHCollisionComponent* _Other);

public:
	void Start() override;
	void Update() override;

public:
	void SetCollisionScale(const float4& _Scale);
	void SetCollisionRotation(const float4& _Rotation);

	std::shared_ptr<GJHCollisionComponent> GetCollision()
	{
		return m_Collision;
	}
};