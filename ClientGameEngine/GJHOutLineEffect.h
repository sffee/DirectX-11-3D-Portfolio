#pragma once
#include <GJHTargetPostEffect.h>

struct OutLineData
{
	float4 LineColor;
	float4 Option;
};

class GJHOutLineEffect : public GJHTargetPostEffect
{
private:
	GJHRenderPlayer m_EffectPlayer;

public:
	OutLineData m_Data;

public:
	GJHOutLineEffect();
	~GJHOutLineEffect();

public:
	GJHOutLineEffect(const GJHOutLineEffect& _Other) = delete;
	GJHOutLineEffect(const GJHOutLineEffect&& _Other) = delete;

public:
	GJHOutLineEffect operator=(const GJHOutLineEffect& _Other) = delete;
	GJHOutLineEffect operator=(const GJHOutLineEffect&& _Other) = delete;

public:
	void Start() override;
	void Effect() override;
};