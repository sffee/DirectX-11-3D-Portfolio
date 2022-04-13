#include "GJHParticleRenderer.h"
#include <GJHGameEngineRandom.h>
#include <GJHGameEngineTime.h>

GJHParticleRenderer::GJHParticleRenderer() :
	m_CreateCount(2),
	m_CreateTime(0.5f),
	m_ActiveCount(0),
	m_ParticleCount(10),
	m_CurCreateTime(0.5f),
	m_LifeTimeMin(1.f),
	m_LifeTimeMax(1.f)
{

}

GJHParticleRenderer::~GJHParticleRenderer()
{

}

void GJHParticleRenderer::Start(int _Order)
{
	GJHRenderer::Start(_Order);

	m_ArrParticleData.resize(m_ParticleCount);

	m_CurCreateTime = m_CreateTime;
	m_CreateCount = 2;

	for (int i = 0; i < m_ArrParticleData.size(); i++)
	{
		m_ArrParticleData[i].Time.x = 0.f;
	}

	m_LifeTimeMin = 1.f;
	m_LifeTimeMax = 1.f;

	std::shared_ptr<GJHRenderPlayer> Render = CreateRenderPlayer(L"Particle", L"Point");

	Render->SetRenderType(RENDERINGTYPE::INSTANCE);
	Render->SetInstanceCount(m_ParticleCount);
	Render->SBufferSettingLink(L"ArrParticleData", m_ArrParticleData);
	Render->TextureSetting(L"Tex", L"HardCircle.png");

	CreateUpdate();
}

void GJHParticleRenderer::ParticleDataReset(ParticleData& _Data, int _Index)
{
	_Data.Pos = { GJHGameEngineRandom::RandomFloat(-1.f, 1.f), 0.f, -_Index * 0.01f };
	
	float Scale = GJHGameEngineRandom::RandomFloat(0.5f, 1.f);
	_Data.Scale = { Scale, Scale, Scale };
	_Data.Dir = { 0.f, 1.f, 0.f };
	_Data.Time.x = GJHGameEngineRandom::RandomFloat(m_LifeTimeMin, m_LifeTimeMax);
	_Data.Time.y = GJHGameEngineRandom::RandomFloat(1.f, 2.f);

	++m_ActiveCount;
}

void GJHParticleRenderer::ParticleDataUpdate(ParticleData& _Data, int _Index)
{
	_Data.Pos = _Data.Pos + (_Data.Dir * GJHGameEngineTime::FDeltaTime() * _Data.Time.y);
	_Data.Time.x -= GJHGameEngineTime::FDeltaTime();

	if (_Data.Time.x <= 0.f)
	{
		--m_ActiveCount;
	}
}

void GJHParticleRenderer::CreateUpdate()
{
	m_CurCreateCount = m_CreateCount;

	for (int i = 0; i < m_ParticleCount; i++)
	{
		if (0.f < m_ArrParticleData[i].Time.x)
		{
			continue;
		}

		ParticleDataReset(m_ArrParticleData[i], i);
		--m_CurCreateCount;

		if (m_CurCreateCount == 0 || m_ParticleCount <= m_ActiveCount)
		{
			break;
		}
	}

	m_CurCreateTime = m_CreateTime;
}

void GJHParticleRenderer::Update()
{
	m_CurCreateTime -= GJHGameEngineTime::FDeltaTime();

	if (m_ActiveCount < m_ParticleCount
		&& m_CurCreateTime <= 0)
	{
		CreateUpdate();
	}

	for (int i = 0; i < m_ArrParticleData.size(); i++)
	{
		if (m_ArrParticleData[i].Time.x <= 0.f)
		{
			continue;
		}

		ParticleDataUpdate(m_ArrParticleData[i], i);
	}
}