#pragma once
#include <thread>
#include <mutex>
#include <map>
#include "GJHGameEngineString.h"
#include "GJHGameEngineMemory.h"
#include "GJHGameEngineDebug.h"
#include "GJHNameBase.h"

class GJHGameEngineThread : public GJHNameBase
{
private:
	static std::mutex Lock;
	static std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineThread>> m_ThreadMap;

public:
	static int GetCoreCount() 
	{
		SYSTEM_INFO Info;
		GetSystemInfo(&Info);
		return Info.dwNumberOfProcessors;
	}

private:
	static std::shared_ptr<GJHGameEngineThread> FindThread(const GJHGameEngineString& _ThreadName)
	{
		auto Var = m_ThreadMap.find(_ThreadName);

		if (Var == m_ThreadMap.end())
		{
			return nullptr;
		}

		return Var->second;
	}

public:
	template<typename ... REST>
	static void CreateThread(const GJHGameEngineString& _ThreadName, REST&& ... _Arg)
	{
		std::lock_guard L(Lock);

		if (FindThread(_ThreadName) != nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (FindThread(_ThreadName) != nullptr)");
			return;
		}

		m_ThreadMap.insert(std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineThread>>::value_type(_ThreadName, std::make_shared<GJHGameEngineThread>(_Arg...)));
	}

private:
	std::shared_ptr<std::thread> ThreadPtr;

public:
	template<typename Func, typename ... REST>
	void Start(Func _Ptr, REST&& ... _Arg)
	{
		ThreadPtr = std::make_shared<std::thread>(_Ptr, _Arg...);
	}

public:
	void Wait()
	{
		ThreadPtr->join();
	}

public:
	GJHGameEngineThread()
	{
	}

	template<typename ... REST>
	GJHGameEngineThread(REST&& ... _Arg)
	{
		ThreadPtr = std::make_shared<std::thread>(_Arg...);
	}

	~GJHGameEngineThread();

public:
	GJHGameEngineThread(const GJHGameEngineThread& _Other) = delete;
	GJHGameEngineThread(const GJHGameEngineThread&& _Other) = delete;

public:
	GJHGameEngineThread operator=(const GJHGameEngineThread& _Other) = delete;
	GJHGameEngineThread operator=(const GJHGameEngineThread&& _Other) = delete;
};