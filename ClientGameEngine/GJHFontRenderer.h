#pragma once
#include "GJHRenderer.h"
#include <GJHDirectFont.h>

enum FONTPIVOT
{
	LT
};

class GJHFontRenderer : public GJHRenderer
{
private:
	std::shared_ptr<GJHDirectFont> m_Font;
	GJHGameEngineString m_Text;
	std::shared_ptr<GJHDirectRenderTarget> m_Target;
	bool m_TextCheck;
	
	float4 m_CutData;
	float4 m_TargetScale;
	float4 m_TextScale;
	float m_Scale;

public:
	void FontSetting(const GJHGameEngineString& _Text);
	void SetText(const GJHGameEngineString& _Text, float _FontScale, float _RatioScale = 1.f, FONTPIVOT _Pivot = FONTPIVOT::LT);
	void SetText(const GJHGameEngineString& _Text, float _RatioScale = 1.f, FONTPIVOT _Pivot = FONTPIVOT::LT);

public:
	float4 GetTextScale()
	{
		return m_TextScale;
	}

public:
	GJHFontRenderer();
	~GJHFontRenderer();

public:
	GJHFontRenderer(const GJHFontRenderer& _Other) = delete;
	GJHFontRenderer(const GJHFontRenderer&& _Other) = delete;

public:
	GJHFontRenderer operator=(const GJHFontRenderer& _Other) = delete;
	GJHFontRenderer operator=(const GJHFontRenderer&& _Other) = delete;

public:
	void Start(int _Order = 0);
	void RenderingSetting() override;
};