#include "GJHFontRenderer.h"
#include <GJHDirectSprite.h>
#include <GJHDirectRenderTarget.h>
#include <GJHGameEngineWindow.h>
#include "GJHCamera.h"

GJHFontRenderer::GJHFontRenderer() :
	m_Scale(30.f)
{

}

GJHFontRenderer::~GJHFontRenderer()
{

}

void GJHFontRenderer::Start(int _Order)
{
	GJHRenderer::Start(_Order);

	m_Target = std::make_shared<GJHDirectRenderTarget>();

	m_Target->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);

	m_TargetScale = GJHGameEngineWindow::GetSize();

	CreateRenderPlayer(L"FontDraw", L"Rect");
	CBufferSettingLink(L"SpriteCutData", m_CutData);
}

void GJHFontRenderer::FontSetting(const GJHGameEngineString& _Text)
{
	m_Font = GJHDirectFont::Find(_Text);
}

void GJHFontRenderer::SetText(const GJHGameEngineString& _Text, float _FontScale, float _RatioScale, FONTPIVOT _Pivot)
{
	m_Scale = _FontScale;
	SetText(_Text, _RatioScale, _Pivot);
}

void GJHFontRenderer::SetText(const GJHGameEngineString& _Text, float _RatioScale, FONTPIVOT _Pivot)
{
	m_Target->Clear();

	m_TextScale.x = m_Scale * _Text.GetSize();
	m_TextScale.y = m_Scale;

	m_Text = _Text;

	m_CutData = { 0.f, 0.f, m_TextScale.x / m_TargetScale.x * 1.2f, m_TextScale.y / m_TargetScale.y * 1.2f };
	m_TextCheck = true;

	float4 RatioScale = m_TextScale * _RatioScale;

	SetLocalPosition({ RatioScale.hx(), -RatioScale.hy() });
	SetLocalScale(RatioScale);
}

void GJHFontRenderer::RenderingSetting()
{
	m_Target->Setting();
	m_Font->DrawFont(m_Text, m_Scale, { 0, 0 });
	GJHGameEngineDevice::Reset();

	m_CurCamera->CamTargetSetting();
	TextureSetting(L"Tex", m_Target->GetTex(0));
}