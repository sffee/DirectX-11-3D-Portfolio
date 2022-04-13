#include "GJHUvdistortion.h"
#include <GJHGameEngineTime.h>

GJHUvDistortion::GJHUvDistortion()
{

}

GJHUvDistortion::~GJHUvDistortion()
{
}

void GJHUvDistortion::Start()
{
    m_EffectPlayer.SetMaterial(L"Uvdistortion");
    m_EffectPlayer.SetMesh(L"FullRect");
    m_EffectPlayer.TextureSetting(L"Tex", m_EffectTarget->GetTex(0));
    m_EffectPlayer.CBufferSettingLink(L"UvdistortionData", m_Speed);
    m_ResultTarget->CreateRenderTarget({ (float)m_EffectTarget->GetTex(0)->GetSizeX(), (float)m_EffectTarget->GetTex(0)->GetSizeY() }, float4::NONE);
    
    m_Speed.x = 2.f;
}
void GJHUvDistortion::Effect()
{
    m_Speed.x += GJHGameEngineTime::FDeltaTime(3.f, false);

    m_ResultTarget->Clear();
    m_ResultTarget->Setting();
    m_EffectPlayer.Render();
    m_EffectTarget->Copy(m_ResultTarget);
}

void GJHUvDistortion::Reset()
{
    m_Speed.x = 2.f;
    m_EffectPlayer.TextureSetting(L"Tex", m_EffectTarget->GetTex(0));
}