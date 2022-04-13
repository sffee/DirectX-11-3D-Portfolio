#include "GJHSpriteRenderer.h"
#include <GJHDirectSprite.h>

GJHSpriteRenderer::GJHSpriteRenderer() :
	CutData(0.f, 0.f, 1.f, 1.f),
	Color(1.f, 1.f, 1.f, 1.f),
	OnlyColor(0.f, 0.f, 0.f, 0.f)
{

}

GJHSpriteRenderer::~GJHSpriteRenderer()
{

}

void GJHSpriteRenderer::Start(int _Order)
{
	GJHRenderer::Start(_Order);

	OnlyColor = float4(0.f, 0.f, 0.f, 0.f);

	CreateRenderPlayer(L"Texture2DMat", L"Rect");
	CBufferSettingLink(L"LastEffectColor", Color);
	CBufferSettingLink(L"LastOnlyColor", OnlyColor);
	CBufferSettingLink(L"SpriteCutData", CutData);
}

void GJHSpriteRenderer::TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize)
{
	CutData = { 0.f, 0.f, 1.f, 1.f };
	GJHRenderer::TextureSetting(L"Tex", _TexName, _AutoSize);
}

void GJHSpriteRenderer::TextureSetting(std::shared_ptr<GJHDirectTexture2D> _Texture, bool _AutoSize)
{
	CutData = { 0.f, 0.f, 1.f, 1.f };
	GJHRenderer::TextureSetting(L"Tex", _Texture, _AutoSize);
}

void GJHSpriteRenderer::SpriteSetting(const GJHGameEngineString& _SpriteName, int _Index)
{
	std::shared_ptr<GJHDirectSprite> Sprite = GJHDirectSprite::Find(_SpriteName);
	if (Sprite == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Sprite == nullptr)");
	}

	CutData = Sprite->GetCutData(_Index);
	GJHRenderer::TextureSetting(L"Tex", Sprite->GetTexture());
}

void GJHSpriteRenderer::SpriteSetting(std::shared_ptr<GJHDirectSprite> _Sprite, int _Index)
{
	CutData = _Sprite->GetCutData(_Index);
	GJHRenderer::TextureSetting(L"Tex", _Sprite->GetTexture());
}