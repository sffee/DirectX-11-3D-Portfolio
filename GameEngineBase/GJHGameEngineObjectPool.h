#pragma once

#include <vector>
#include <list>
#include "GJHGameEngineMemory.h"
#include "GJHGameEngineDebug.h"

template<typename ObjectType, int PoolSize = 1024>
class GJHGameEngineObjectPool
{
private:
	std::list<SharedPtr<ObjectType>> m_ObjectPool;

public:
	std::list<SharedPtr<ObjectType>> ActiveObject;

public:
	SharedPtr<ObjectType> PopObject()
	{
		if (m_ObjectPool.size() <= 0)
		{
			MsgAssert("if (m_ObjectPool.size() <= 0)");
			return nullptr;
		}

		SharedPtr<ObjectType> Ptr = *m_ObjectPool.begin();
		m_ObjectPool.pop_front();
		ActiveObject.push_back(Ptr);

		return Ptr;
	}

public:
	void ObjectMemoryCheck()
	{
		typename std::list<SharedPtr<ObjectType>>::iterator Start = ActiveObject.begin();
		typename std::list<SharedPtr<ObjectType>>::iterator End = ActiveObject.end();

		for (; Start != End;)
		{
#ifdef _DEBUG
			int CountTest = (*Start).use_count();
#endif

			if ((*Start).use_count() == 1)
			{
				m_ObjectPool.push_back(*Start);
				Start = ActiveObject.erase(Start);
			}
			else
			{
				++Start;
			}
		}
	}

public:
	template<typename ... REST>
	GJHGameEngineObjectPool(REST ... _Arg)
	{
		for (size_t i = 0; i < PoolSize; i++)
		{
			m_ObjectPool.push_back(std::make_shared<ObjectType>(_Arg...));
		}
	}

	~GJHGameEngineObjectPool()
	{

	}

public:
	GJHGameEngineObjectPool(const GJHGameEngineObjectPool& _Other) = delete;
	GJHGameEngineObjectPool(const GJHGameEngineObjectPool&& _Other) = delete;

public:
	GJHGameEngineObjectPool& operator=(const GJHGameEngineObjectPool& _Other) = delete;
	GJHGameEngineObjectPool& operator=(const GJHGameEngineObjectPool&& _Other) = delete;
};