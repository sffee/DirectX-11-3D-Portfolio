#pragma once
#include <GJHTargetPostEffect.h>

struct UvdistortionData
{
	float4 TimeValue;
	float4 TileFactor;
};

class GJHDefferdLighting : public GJHTargetPostEffect
{
private:
	GJHRenderPlayer m_EffectPlayer;

	GJHRenderPlayer m_MergeRenderer;
	std::shared_ptr<GJHDirectRenderTarget> m_DefferdTarget;

public:
	std::shared_ptr<GJHDirectRenderTarget> GetResultTarget()
	{
		return m_ResultTarget;
	}

	std::shared_ptr<GJHDirectRenderTarget> GetDefferdTarget()
	{
		return m_DefferdTarget;
	}

	GJHRenderPlayer& GetLightingPlayer()
	{
		return m_EffectPlayer;
	}

public:
	GJHDefferdLighting();
	~GJHDefferdLighting();

public:
	GJHDefferdLighting(const GJHDefferdLighting& _Other) = delete;
	GJHDefferdLighting(const GJHDefferdLighting&& _Other) = delete;

public:
	GJHDefferdLighting& operator=(const GJHDefferdLighting& _Other) = delete;
	GJHDefferdLighting& operator=(const GJHDefferdLighting&& _Other) = delete;

public:
	void Start() override;
	void Effect() override;

public:
	void DefferdMerge();
};

