#pragma once
#include <map>
#include <GJHGameEngineString.h>
#include <memory>
#include "GJHResBase.h"
#include <GJHGameEngineDebug.h>
#include <thread>
#include <mutex>

template<typename T>
class GJHDirectResourcesManager
{
private:
	static std::mutex ResLock;
	static std::map<GJHGameEngineString, std::shared_ptr<T>> m_ResMap;

public:
	GJHDirectResourcesManager() { }
	~GJHDirectResourcesManager() { }
	GJHDirectResourcesManager(const GJHDirectResourcesManager& _Other) = delete;
	GJHDirectResourcesManager(const GJHDirectResourcesManager&& _Other) = delete;

public:
	GJHDirectResourcesManager operator=(const GJHDirectResourcesManager& _Other) = delete;
	GJHDirectResourcesManager operator=(const GJHDirectResourcesManager&& _Other) = delete;

public:
	static std::shared_ptr<T> Find(const GJHGameEngineString& _Name)
	{
		if (m_ResMap.find(_Name) == m_ResMap.end())
		{
			return nullptr;
		}

		return m_ResMap[_Name];
	}

	static std::shared_ptr<T> FindAsync(const GJHGameEngineString& _Name)
	{
		ResLock.lock();

		if (m_ResMap.find(_Name) == m_ResMap.end())
		{
			ResLock.unlock();
			return nullptr;
		}

		std::shared_ptr<T> Return = m_ResMap[_Name];

		ResLock.unlock();

		return Return;
	}

	template <typename ConvertType>
	static std::shared_ptr<ConvertType> FindAndCasting(const GJHGameEngineString& _Name)
	{
		return std::dynamic_pointer_cast<ConvertType, GJHResBase>(Find(_Name));
	}

protected:
	static std::shared_ptr<T> InsertRes(const GJHGameEngineString& _Name, T* _Res)
	{
		if (m_ResMap.find(_Name) != m_ResMap.end())
		{
			GJHGameEngineDebug::AssertMsg("if (m_ResMap.find(_Name) != m_ResMap.end())");
		}

		std::shared_ptr<T> ResSharedPtr(_Res);
		m_ResMap[_Name] = ResSharedPtr;

		return ResSharedPtr;
	}

	static void InsertResSheard(GJHGameEngineString _Name, std::shared_ptr<T> pRes)
	{
		if (m_ResMap.end() != m_ResMap.find(_Name))
		{
			GJHGameEngineDebug::AssertMsg("if (m_ResMap.end() != m_ResMap.find( _Name))");
		}

		m_ResMap[_Name] = pRes;
		return;
	}

	static std::shared_ptr<T> InsertResAsync(GJHGameEngineString _Name, T* pRes)
	{
		ResLock.lock();

		if (m_ResMap.find(_Name) != m_ResMap.end())
		{
			GJHGameEngineDebug::AssertMsg("if (m_ResMap.find(_Name) != m_ResMap.end())");
		}

		std::shared_ptr<T> ResSharedPtr(pRes);
		m_ResMap[_Name] = ResSharedPtr;

		ResLock.unlock();

		return ResSharedPtr;
	}

	static void InsertResSheardAsync(GJHGameEngineString _Name, std::shared_ptr<T> pRes)
	{
		ResLock.lock();

		if (m_ResMap.find(_Name) != m_ResMap.end())
		{
			GJHGameEngineDebug::AssertMsg("if (m_ResMap.find(_Name) != m_ResMap.end())");
		}

		m_ResMap[_Name] = pRes;

		ResLock.unlock();

		return;
	}

public:
	static void Release()
	{
		m_ResMap.clear();
		return;
	}
};

template<typename T>
std::mutex GJHDirectResourcesManager<T>::ResLock;

template<typename T>
std::map<GJHGameEngineString, std::shared_ptr<T>> GJHDirectResourcesManager<T>::m_ResMap;