#pragma once
#include <BasePre.h>
#include <GJHGameEngineObjectPool.h>
#include <set>
#include <map>
#include "GJHGameEngineCollision.h"

class GJHGameEngineCollision;
class GJHGameEngineTransform;
class GJHGameEngineWorld : public SharedPtrThis<GJHGameEngineWorld>
{
private:
	friend GJHGameEngineTransform;

private:
	bool m_bParentTransCheck;

	void ParentTransCheckOn()
	{
		m_bParentTransCheck = true;
	}

private:
	GJHGameEngineObjectPool<GJHGameEngineTransform, 10000> m_TransPool;
	std::list<std::shared_ptr<GJHGameEngineTransform>> m_ActiveList;

public:
	std::map<int, std::list<std::shared_ptr<GJHGameEngineCollision>>> m_ColGroup;
	std::set<int> m_ColUpdateSet;

	std::map<int, std::set<int>> m_ColLink;

public:
	void SetCollisionLink(int _Left, int _Right);

	std::shared_ptr<GJHGameEngineCollision> CreateCollision(int _Order, COLTYPE _Type, GJHGameEngineTransform& _Collision);
	std::list<std::shared_ptr<GJHGameEngineCollision>>& GetCollisionGroup(int _Order);
	std::list<std::shared_ptr<GJHGameEngineCollision>> GetCollisionCheck(std::shared_ptr<GJHGameEngineCollision> _Trans, COLTYPE _Coltype, float4 MovePos, int _Order);
	


public:
	void TransUpdate();


public:
	GJHGameEngineWorld();
	~GJHGameEngineWorld();

public:
	GJHGameEngineWorld(const GJHGameEngineWorld& _Other) = delete;
	GJHGameEngineWorld(const GJHGameEngineWorld&& _Other) = delete;

public:
	GJHGameEngineWorld operator=(const GJHGameEngineWorld& _Other) = delete;
	GJHGameEngineWorld operator=(const GJHGameEngineWorld&& _Other) = delete;

public:
	void Release();

public:
	SharedPtr<GJHGameEngineTransform> CreateTrans(bool _ActiveList = true);
	void PushTrans(SharedPtr<GJHGameEngineTransform> _Trans);

public:
	void Collision();
};