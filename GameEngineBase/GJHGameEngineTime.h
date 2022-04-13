#pragma once
#include <Windows.h>
#include <functional>
#include "GJHGameEngineString.h"
#include <vector>

class GJHGameEngineTime
{
private:
	static GJHGameEngineTime Inst;
	static float TimeScale;

public:
	static GJHGameEngineTime& GetInst()
	{
		return Inst;
	}

	static void SetTimeScale(float _Scale);
	static float GetTimeScale();

	static float FDeltaTime(float _Speed = 1.f, bool _UseTimeScale = true);
	static double DDeltaTime();

private:
	LARGE_INTEGER m_CountTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_PrevTime;
	double m_DoubleTime;
	float m_FloatTime;

public:
	float GetFDeltaTime() {
		return m_FloatTime;
	}

public:
	GJHGameEngineTime();
	~GJHGameEngineTime();
	GJHGameEngineTime(const GJHGameEngineTime& _Other) = delete;
	GJHGameEngineTime(const GJHGameEngineTime&& _Other) = delete;

public:
	GJHGameEngineTime operator=(const GJHGameEngineTime& _Other) = delete;
	GJHGameEngineTime operator=(const GJHGameEngineTime&& _Other) = delete;

public:
	void Reset();
	void Update();
};

class GJHTimeEventPlayer
{
public:
	class TimeEvent
	{
	public:
		float m_Time;
		float m_MaxTime;
		std::function<void()> m_StartEvent;
		std::function<void()> m_StayEvent;
		std::function<void()> m_EndEvent;

	public:
		void Reset()
		{
			m_Time = m_MaxTime;
		}

		bool Update()
		{
			if (m_Time == m_MaxTime)
			{
				m_StartEvent();
				m_Time -= GJHGameEngineTime::GetInst().FDeltaTime();
				
				return false;
			}

			m_Time -= GJHGameEngineTime::GetInst().FDeltaTime();

			if (0 < m_Time)
			{
				m_StayEvent();
				return false;
			}
			else
			{
				m_EndEvent();
				return true;
			}
		}
	};

private:
	std::vector<TimeEvent> m_EventVector;
	int m_CurIndex;
	bool m_IsLoop;

public:
	GJHTimeEventPlayer() :
		m_CurIndex(0),
		m_IsLoop(false)
	{
	}

	~GJHTimeEventPlayer()
	{
	}

private:
	void CreateTimeEventStart(float _Time, std::function<void()> _StartEvent, std::function<void()> _StayEvent = nullptr, std::function<void()> _EndEvent = nullptr)
	{
		m_EventVector.push_back({ _Time, _Time, _StartEvent, _StayEvent, _EndEvent });
	}

	void CreateTimeEventStay(float _Time, std::function<void()> _StayEvent, std::function<void()> _StartEvent = nullptr, std::function<void()> _EndEvent = nullptr)
	{
		m_EventVector.push_back({ _Time, _Time, _StartEvent, _StayEvent, _EndEvent });
	}

	void CreateTimeEventEnd(float _Time, std::function<void()> _EndEvent, std::function<void()> _StayEvent = nullptr, std::function<void()> _StartEvent = nullptr)
	{
		m_EventVector.push_back({ _Time, _Time, _StartEvent, _StayEvent, _EndEvent });
	}

public:
	void Reset()
	{
		m_CurIndex = 0;
		for (size_t i = 0; i < m_EventVector.size(); i++)
		{
			m_EventVector[i].Reset();
		}
	}

	void Update()
	{
		if (m_EventVector.size() <= m_CurIndex)
		{
			return;
		}

		if (m_EventVector[m_CurIndex].Update() == true)
		{
			++m_CurIndex;
			if (m_EventVector.size() <= m_CurIndex)
			{
				if (m_IsLoop == true)
				{
					Reset();
				}
			}
		}
	}
};