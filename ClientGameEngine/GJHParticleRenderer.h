#pragma once
#include "GJHRenderer.h"

struct ParticleData
{
	float4 Pos;
	float4 Dir;
	float4 Scale;
	float4 Time;
};

enum class PARTICLEUPDATEMODE
{
	COMPUT,
	CPU
};

class GJHParticleRenderer : public GJHRenderer
{
public:
	int m_ParticleCount;
	int m_ActiveCount;

	float m_LifeTimeMin;
	float m_LifeTimeMax;

	float m_CreateTime;
	float m_CurCreateTime;

	int m_CreateCount;
	int m_CurCreateCount;

private:
	std::vector<ParticleData> m_ArrParticleData;

private:
	void ParticleDataReset(ParticleData& _Data, int _Index);
	void ParticleDataUpdate(ParticleData& _Data, int _Index);

public:
	GJHParticleRenderer();
	~GJHParticleRenderer();

public:
	GJHParticleRenderer(const GJHParticleRenderer& _Other) = delete;
	GJHParticleRenderer(const GJHParticleRenderer&& _Other) = delete;

public:
	GJHParticleRenderer operator=(const GJHParticleRenderer& _Other) = delete;
	GJHParticleRenderer operator=(const GJHParticleRenderer&& _Other) = delete;

public:
	void Start(int _Order = 0);
	void Update() override;

public:
	void CreateUpdate();
};