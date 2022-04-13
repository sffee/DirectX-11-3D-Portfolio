#pragma once

#include <vector>
#include <list>
#include <map>
#include "GJHGameEngineString.h"
#include "GJHGameEngineMemory.h"

class GJHGameEngineInput
{
private:
	class GJHGameEngineKey : public SharedPtrThis<GJHGameEngineKey>
	{
	private:
		bool m_Down;
		bool m_Press;
		bool m_Up;
		bool m_Free;

		float PressTime;
		float UpTime;

		std::vector<int> m_CheckKey;

		friend GJHGameEngineInput;

	public:
		template<typename ... REST>
		void PushKey(int _CurKey, REST ... _Arg)
		{
			m_CheckKey.push_back(_CurKey);
			PushKey(_Arg...);
		}

		void PushKey()
		{
		}

	private:
		bool KeyCheck();
		void Update();
		void Reset();
	};

	class GJHCommandKey : public SharedPtrThis<GJHCommandKey>
	{
		friend GJHGameEngineInput;

		std::list<std::shared_ptr<GJHGameEngineKey>> m_Key;
		std::list<float> m_WaitNext;

		std::list<std::shared_ptr<GJHGameEngineKey>> m_CurCheckKey;
		std::list<float> m_CurWaitNext;

		bool IsResult;

	private:
		void Update();

	public:
		GJHCommandKey() 
			: IsResult(false)
		{

		}
	};

private:
	static GJHGameEngineInput Inst;

public:
	static GJHGameEngineInput& GetInst()
	{
		return Inst;
	}

private:
	static bool m_IsMouseMoveToCenter;

public:
	static void SetMouseMoveToCenter(bool _Set)
	{
		m_IsMouseMoveToCenter = _Set;
	}

	static void ToggleMouseMoveToCenter()
	{
		m_IsMouseMoveToCenter = !m_IsMouseMoveToCenter;
	}

public:
	static bool IsKey(const GJHGameEngineString& _Name);
	static bool Down(const GJHGameEngineString& _Name);
	static bool Up(const GJHGameEngineString& _Name);
	static bool Press(const GJHGameEngineString& _Name);
	static bool Free(const GJHGameEngineString& _Name);

	static bool CommandCheck(const GJHGameEngineString& _Name);

private:
	std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineKey>> m_AllKey;

	std::map<GJHGameEngineString, std::shared_ptr<GJHCommandKey>> m_AllCommandKey;

	typedef std::shared_ptr<GJHGameEngineKey> KeyPtr;
	typedef std::shared_ptr<GJHCommandKey> CommandPtr;

public:
	GJHGameEngineInput();
	~GJHGameEngineInput();
	GJHGameEngineInput(const GJHGameEngineInput& _Other) = delete;
	GJHGameEngineInput(const GJHGameEngineInput&& _Other) = delete;

public:
	GJHGameEngineInput operator=(const GJHGameEngineInput& _Other) = delete;
	GJHGameEngineInput operator=(const GJHGameEngineInput&& _Other) = delete;

private:
	std::shared_ptr<GJHGameEngineKey> FindKey(const GJHGameEngineString& _KeyName);
	std::shared_ptr<GJHCommandKey> FindCommandKey(const GJHGameEngineString& _KeyName);

public:
	template<typename ... REST>
	void CreateCommand(const GJHGameEngineString& _KeyName)
	{
		std::shared_ptr<GJHCommandKey> Key = FindCommandKey(_KeyName);

		if (Key != nullptr)
		{
			assert("if (FindKey(_KeyName) != nullptr)");
			return;
		}

		std::shared_ptr<GJHCommandKey> NewKey = std::make_shared<GJHCommandKey>();
		m_AllCommandKey.insert(std::map<GJHGameEngineString, std::shared_ptr<GJHCommandKey>>::value_type(_KeyName, NewKey));
	}

	template<typename ... REST>
	void AddCommmandKey(const GJHGameEngineString& _KeyName, float _Time, REST ... _Arg)
	{
		std::shared_ptr<GJHCommandKey> Key = FindCommandKey(_KeyName);

		if (Key == nullptr)
		{
			assert("if (FindKey(_KeyName) == nullptr)");
			return;
		}

		std::shared_ptr<GJHGameEngineKey> NewKey = std::make_shared<GJHGameEngineKey>();
		NewKey->Reset();
		NewKey->PushKey(_Arg...);

		Key->m_Key.push_back(NewKey);
		Key->m_WaitNext.push_back(_Time);
	}


	template<typename ... REST>
	void CreateKey(const GJHGameEngineString& _KeyName, REST ... _Arg)
	{
		if (FindKey(_KeyName) != nullptr)
		{
			assert("if (FindKey(_KeyName) != nullptr)");
			return;
		}

		std::shared_ptr<GJHGameEngineKey> NewKey = std::make_shared<GJHGameEngineKey>();
		NewKey->Reset();
		NewKey->PushKey(_Arg...);
		m_AllKey.insert(std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineKey>>::value_type(_KeyName, NewKey));
	}

	void Update();

private:
	float4 m_MousePos;
	float4 m_MousePos3D;
	float4 m_PrevMousePos3D;
	float4 m_MousePos3DDistance;
	float4 m_MouseDir3D;

public:
	static float4 GetMousePos();
	static float4 GetMouse3DPos();
	static float4 GetMouse3DDistance();
	static float4 GetMouse3DDir();

public:
	static void MouseMoveToCenter();
};