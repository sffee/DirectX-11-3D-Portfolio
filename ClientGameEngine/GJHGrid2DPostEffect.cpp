#include "GJHGrid2DPostEffect.h"

GJHGrid2DPostEffect::GJHGrid2DPostEffect()
{

}

GJHGrid2DPostEffect::~GJHGrid2DPostEffect()
{

}

void GJHGrid2DPostEffect::Start()
{
	m_EffectPlayer.SetMaterial(L"Grid2D");
	m_EffectPlayer.SetMesh(L"FullRect");
	m_EffectPlayer.TextureSetting(L"Tex", m_EffectTarget->GetTex(0));
	m_ResultTarget->CreateRenderTarget({ (float)m_EffectTarget->GetTex(0)->GetSizeX(), (float)m_EffectTarget->GetTex(0)->GetSizeY() }, float4::NONE);
}

void GJHGrid2DPostEffect::Effect()
{
	m_ResultTarget->Setting();

	m_EffectPlayer.Render();

	m_EffectTarget->Copy(m_ResultTarget);
}