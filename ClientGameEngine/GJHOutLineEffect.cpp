#include "GJHOutLineEffect.h"

GJHOutLineEffect::GJHOutLineEffect()
{

}

GJHOutLineEffect::~GJHOutLineEffect()
{

}

void GJHOutLineEffect::Start()
{
	m_Data.LineColor = { 1.f, 0.f, 1.f, 1.f };
	m_Data.Option.x = 5.f;

	m_EffectPlayer.SetMaterial(L"OutLine");
	m_EffectPlayer.SetMesh(L"FullRect");
	m_EffectPlayer.TextureSetting(L"Tex", m_EffectTarget->GetTex(0));
	m_EffectPlayer.CBufferSettingLink(L"OutLineData", m_Data);
	m_ResultTarget->CreateRenderTarget({ (float)m_EffectTarget->GetTex(0)->GetSizeX(), (float)m_EffectTarget->GetTex(0)->GetSizeY() }, float4::NONE);
}

void GJHOutLineEffect::Effect()
{
	m_ResultTarget->Clear();
	m_ResultTarget->Setting();
	m_EffectPlayer.Render();
	m_EffectTarget->Copy(m_ResultTarget);
}