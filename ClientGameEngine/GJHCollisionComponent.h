#pragma once
#include "GJHActorTransform.h"
#include <GJHGameEngineCollision.h>

class GJHRenderer;
class GJHWorldComponent;
class GJHCollisionComponent : public GJHActorTransform
{
private:
	friend GJHWorldComponent;

	std::shared_ptr<GJHGameEngineCollision> m_Collision;
	std::shared_ptr<GJHRenderer> m_DebugRender;

private:
	std::set<GJHCollisionComponent*> m_OtherCol;
	std::vector<std::function<void(GJHCollisionComponent*)>> m_EnterList;
	std::vector<std::function<void(GJHCollisionComponent*)>> m_StayList;
	std::vector<std::function<void(GJHCollisionComponent*)>> m_ExitList;

public:
	template<typename EventClass>
	void AddEnterEvent(void(EventClass::* _Func)(GJHCollisionComponent*, GJHCollisionComponent*), EventClass& _Obj)
	{
		if (_Func == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_Func == nullptr)");
		}

		m_EnterList.push_back(std::bind(_Func, &_Obj, this, std::placeholders::_1));
	}

	template<typename EventClass>
	void AddStayEvent(void(EventClass::* _Func)(GJHCollisionComponent*, GJHCollisionComponent*), EventClass& _Obj)
	{
		if (_Func == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_Func == nullptr)");
		}

		m_StayList.push_back(std::bind(_Func, &_Obj, this, std::placeholders::_1));
	}

	template<typename EventClass>
	void AddExitEvent(void(EventClass::* _Func)(GJHCollisionComponent*, GJHCollisionComponent*), EventClass& _Obj)
	{
		if (_Func == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_Func == nullptr)");
		}

		m_ExitList.push_back(std::bind(_Func, &_Obj, this, std::placeholders::_1));
	}

	void AddEnterEvent(void(*_Func)(GJHCollisionComponent*, GJHCollisionComponent*))
	{
		if (_Func == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_Func == nullptr)");
		}

		m_EnterList.push_back(std::bind(_Func, this, std::placeholders::_1));
	}

	void AddStayEvent(void(*_Func)(GJHCollisionComponent*, GJHCollisionComponent*))
	{
		if (_Func == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_Func == nullptr)");
		}

		m_StayList.push_back(std::bind(_Func, this, std::placeholders::_1));
	}

	void AddExitEvent(void(*_Func)(GJHCollisionComponent*, GJHCollisionComponent*))
	{
		if (_Func == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_Func == nullptr)");
		}

		m_ExitList.push_back(std::bind(_Func, this, std::placeholders::_1));
	}

private:
	void CallEnter(GJHCollisionComponent* _Other);
	void CallStay(GJHCollisionComponent* _Other);
	void CallExit(GJHCollisionComponent* _Other);

public:
	void ColTrue(GJHGameEngineCollision* _Other);
	void ColFalse(GJHGameEngineCollision* _Other);

public:
	int GetOrder();
	void SetLocalPosition(const float4& _Pos);
	void SetLocalMove(const float4& _Pos);
	void SetLocalScale(const float4& _Scale);
	void SetLocalRotation(const float4& _Rotation);

public:
	GJHCollisionComponent();
	~GJHCollisionComponent();

public:
	GJHCollisionComponent(const GJHCollisionComponent& _Other) = delete;
	GJHCollisionComponent(const GJHCollisionComponent&& _Other) = delete;

public:
	GJHCollisionComponent operator=(const GJHCollisionComponent& _Other) = delete;
	GJHCollisionComponent operator=(const GJHCollisionComponent&& _Other) = delete;

public:
	void Start(int _Order, COLTYPE _Type);
};

using CollisionPtr = std::shared_ptr<GJHCollisionComponent>;