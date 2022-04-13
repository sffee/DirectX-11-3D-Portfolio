#pragma once
#include <GJHTargetPostEffect.h>

class GJHGrid2DPostEffect : public GJHTargetPostEffect
{
private:
	GJHRenderPlayer m_EffectPlayer;

public:
	GJHGrid2DPostEffect();
	~GJHGrid2DPostEffect();

public:
	GJHGrid2DPostEffect(const GJHGrid2DPostEffect& _Other) = delete;
	GJHGrid2DPostEffect(const GJHGrid2DPostEffect&& _Other) = delete;

public:
	GJHGrid2DPostEffect operator=(const GJHGrid2DPostEffect& _Other) = delete;
	GJHGrid2DPostEffect operator=(const GJHGrid2DPostEffect&& _Other) = delete;

public:
	void Start() override;
	void Effect() override;
};