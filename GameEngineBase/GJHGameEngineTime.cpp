#include "BasePre.h"
#include "GJHGameEngineTime.h"

GJHGameEngineTime GJHGameEngineTime::Inst = GJHGameEngineTime();
float GJHGameEngineTime::TimeScale = 1.f;

void GJHGameEngineTime::SetTimeScale(float _Scale)
{
	TimeScale = _Scale;
}

float GJHGameEngineTime::GetTimeScale()
{
	return TimeScale;
}

float GJHGameEngineTime::FDeltaTime(float _Speed, bool _UseTimeScale)
{
	return (_UseTimeScale == true) ? GetInst().m_FloatTime * _Speed * TimeScale : GetInst().m_FloatTime * _Speed;
}

double GJHGameEngineTime::DDeltaTime()
{
	return GetInst().m_DoubleTime;
}

GJHGameEngineTime::GJHGameEngineTime() :
	m_CountTime(),
	m_CurTime(),
	m_PrevTime(),
	m_DoubleTime(0.f),
	m_FloatTime(0.f)
{
	Reset();
}

GJHGameEngineTime::~GJHGameEngineTime()
{

}

void GJHGameEngineTime::Reset()
{
	QueryPerformanceFrequency(&m_CountTime);
	QueryPerformanceCounter(&m_PrevTime);
	QueryPerformanceCounter(&m_CurTime);
}

void GJHGameEngineTime::Update()
{
	QueryPerformanceCounter(&m_CurTime);

	m_DoubleTime = ((double)m_CurTime.QuadPart - (double)m_PrevTime.QuadPart) / ((double)m_CountTime.QuadPart);
	m_PrevTime.QuadPart = m_CurTime.QuadPart;
	m_FloatTime = (float)m_DoubleTime;
}