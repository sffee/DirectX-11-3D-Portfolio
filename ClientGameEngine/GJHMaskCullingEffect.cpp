#include "GJHMaskCullingEffect.h"

GJHMaskCullingEffect::GJHMaskCullingEffect()
{

}

GJHMaskCullingEffect::~GJHMaskCullingEffect()
{

}

void GJHMaskCullingEffect::Start()
{
	m_EffectPlayer.SetMaterial(L"MaskCulling");
	m_EffectPlayer.SetMesh(L"FullRect");
	m_EffectPlayer.TextureSetting(L"Tex", m_EffectTarget->GetTex(0));
	m_ResultTarget->CreateRenderTarget({ (float)m_EffectTarget->GetTex(0)->GetSizeX(), (float)m_EffectTarget->GetTex(0)->GetSizeY() }, float4::NONE);
}

void GJHMaskCullingEffect::Effect()
{
	m_ResultTarget->Clear();
	m_ResultTarget->Setting();
	m_EffectPlayer.Render();
	m_EffectTarget->Copy(m_ResultTarget);
}