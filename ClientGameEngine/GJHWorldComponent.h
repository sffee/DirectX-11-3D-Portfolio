#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineWorld.h>
#include <GJHGameEngineTransForm.h>
#include <GJHGameEngineCollision.h>

class GJHCollisionComponent;
class GJHGameEngineTransform;
class GJHWorldComponent : public GJHGameEngineComponent
{
private:
	GJHGameEngineWorld m_World;

public:
	std::shared_ptr<GJHGameEngineCollision> CreateCollision(int _Order, COLTYPE _Type, GJHGameEngineTransform* _Ptr);
	std::list<std::shared_ptr<GJHGameEngineCollision>>& GetCollisionGroup(int _Order);

public:
	GJHWorldComponent();
	~GJHWorldComponent();

public:
	void SetCollisionLink(int _Left, int _Right);
	std::list<std::shared_ptr<GJHGameEngineCollision>> GetCollisionCheck(std::shared_ptr<GJHCollisionComponent> _Origin, float4 MovePos, int _Order);

public:
	std::shared_ptr<GJHGameEngineTransform> PopTransform(bool _IsUpdaterCheck = true);

public:
	GJHWorldComponent(const GJHWorldComponent& _Other) = delete;
	GJHWorldComponent(const GJHWorldComponent&& _Other) = delete;

public:
	GJHWorldComponent operator=(const GJHWorldComponent& _Other) = delete;
	GJHWorldComponent operator=(const GJHWorldComponent&& _Other) = delete;

public:
	void RenderPrev() override;
	void Collision() override;
	void Release() override;
};