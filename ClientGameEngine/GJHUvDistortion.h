#pragma once
#include <GJHTargetPostEffect.h>

class GJHUvDistortion : public GJHTargetPostEffect
{
private:
	GJHRenderPlayer m_EffectPlayer;
	float4 m_Speed;
	float m_SpeedAddValue;

public:
	GJHUvDistortion();
	~GJHUvDistortion();

public:
	GJHUvDistortion(const GJHUvDistortion& _Other) = delete;
	GJHUvDistortion(const GJHUvDistortion&& _Other) = delete;

public:
	GJHUvDistortion& operator=(const GJHUvDistortion& _Other) = delete;
	GJHUvDistortion& operator=(const GJHUvDistortion&& _Other) = delete;

public:
	void Start() override;
	void Effect() override;

public:
	void Reset();
};

