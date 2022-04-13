#pragma once
#include "GJHGameEngineMemory.h"
#include <list>

class GJHUpdaterBase
{
private:
	GJHUpdaterBase* m_Parent;

public:
	void SetParent(GJHUpdaterBase* _Parent)
	{
		m_Parent = _Parent;
	}

	template <typename ParentType>
	ParentType* GetParent()
	{
		return dynamic_cast<ParentType*>(m_Parent);
	}

private:
	bool m_IsDebug;
	bool m_IsDeath;
	bool m_IsUpdate;

public:
	void DebugOn()
	{
		m_IsDebug = true;
	}

	void DebugCheck()
	{
		if (m_IsDebug == true)
		{
			int a = 0;
		}
	}

	void Death()
	{
		m_IsDeath = true;
	}

	void On()
	{
		m_IsUpdate = true;
	}

	void Off()
	{
		m_IsUpdate = false;
	}

	void ToggleOnOff()
	{
		m_IsUpdate = !m_IsUpdate;
	}

	bool IsDeath()
	{
		if (m_Parent == nullptr)
		{
			return m_IsDeath == true;
		}

		return m_IsDeath == true || m_Parent->IsDeath() == true;
	}

	bool IsUpdate()
	{
		if (m_Parent == nullptr)
		{
			return m_IsUpdate == true && m_IsDeath == false;
		}

		return m_IsUpdate == true && m_IsDeath == false && m_Parent->IsUpdate() == true;
	}

public:
	virtual void DeathTask() { }

public:
	GJHUpdaterBase() : m_Parent(nullptr), m_IsDeath(false), m_IsUpdate(true)
	{
	}

	virtual ~GJHUpdaterBase()
	{
	}

public:
	GJHUpdaterBase(const GJHUpdaterBase& _Other) = delete;
	GJHUpdaterBase(const GJHUpdaterBase&& _Other) = delete;

public:
	GJHUpdaterBase operator=(const GJHUpdaterBase& _Other) = delete;
	GJHUpdaterBase operator=(const GJHUpdaterBase&& _Other) = delete;

public:
	template<typename UpdaterType>
	static void ReleaseList(std::list<UpdaterType>& _List)
	{
		typename std::list<UpdaterType>::iterator Start = _List.begin();
		typename std::list<UpdaterType>::iterator End = _List.end();

		for (; Start != End; )
		{
			if ((*Start)->IsDeath() == true)
			{
				(*Start)->DeathTask();
				Start = _List.erase(Start);
			}
			else
			{
				++Start;
			}
		}
	}
};