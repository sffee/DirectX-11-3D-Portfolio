#pragma once
#include <GJHTargetPostEffect.h>
#include "GJHRenderManagerComponent.h"
#include <GJHDirectRenderTarget.h>

class GJHMaskCullingEffect : public GJHTargetPostEffect
{
private:
	GJHRenderPlayer m_EffectPlayer;
	std::shared_ptr<GJHDirectRenderTarget> m_Target;

public:
	void SetMaksTarget(int _Index)
	{
		m_Target = GJHRenderManagerComponent::GetMask(_Index);
		m_EffectPlayer.TextureSetting(L"MaskTex", m_Target->GetTex(0));
	}

public:
	GJHMaskCullingEffect();
	~GJHMaskCullingEffect();

public:
	GJHMaskCullingEffect(const GJHMaskCullingEffect& _Other) = delete;
	GJHMaskCullingEffect(const GJHMaskCullingEffect&& _Other) = delete;

public:
	GJHMaskCullingEffect operator=(const GJHMaskCullingEffect& _Other) = delete;
	GJHMaskCullingEffect operator=(const GJHMaskCullingEffect&& _Other) = delete;

public:
	void Start() override;
	void Effect() override;
};