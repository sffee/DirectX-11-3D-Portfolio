#include "GJHDefferdLighting.h"
#include <GJHGameEngineTime.h>
#include <GJHGameEngineWindow.h>

GJHDefferdLighting::GJHDefferdLighting()
{

}

GJHDefferdLighting::~GJHDefferdLighting()
{
}

void GJHDefferdLighting::Start()
{
    m_MergeRenderer.SetMaterial(L"DefferdMerge");
    m_MergeRenderer.SetMesh(L"FullRect");
    m_MergeRenderer.TextureSetting(L"Dif", m_EffectTarget->GetTex(0));
    m_MergeRenderer.TextureSetting(L"DifLight", m_ResultTarget->GetTex(0));
    m_MergeRenderer.TextureSetting(L"SpcLight", m_ResultTarget->GetTex(1));
    m_MergeRenderer.TextureSetting(L"AmbLight", m_ResultTarget->GetTex(2));
    m_DefferdTarget = std::make_shared<GJHDirectRenderTarget>();
    m_DefferdTarget->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);

    m_EffectPlayer.SetMaterial(L"DefferdLighting");
    m_EffectPlayer.SetMesh(L"FullRect");

    m_EffectPlayer.SamplerSetting("ShadowSmp", "SHADOWSMP");

    m_EffectPlayer.TextureSetting(L"Pos", m_EffectTarget->GetTex(1));
    m_EffectPlayer.TextureSetting(L"Nor", m_EffectTarget->GetTex(2));
}

void GJHDefferdLighting::Effect()
{
    m_ResultTarget->Setting();
    m_EffectPlayer.Render();
    m_EffectPlayer.AllTextureReset();
}

void GJHDefferdLighting::DefferdMerge()
{
    m_DefferdTarget->Clear();
    m_DefferdTarget->Setting();
    m_MergeRenderer.Render();
}