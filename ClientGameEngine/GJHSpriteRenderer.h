#pragma once
#include "GJHRenderer.h"

class GJHDirectSprite;
class GJHSpriteRenderer : public GJHRenderer
{
private:
	float4 CutData;
	float4 Color;
	float4 OnlyColor;

public:
	void SetColor(float4 _Color)
	{
		Color = _Color;
	}

	float4 GetColor()
	{
		return Color;
	}

	void SetColor(float _Color)
	{
		Color = float4(_Color, _Color, _Color, _Color);
	}

	void SetAlpha(float _Alpha)
	{
		Color = float4(Color.x, Color.y, Color.z, _Alpha);
	}

	float GetAlpha()
	{
		return Color.a;
	}

	void SetOnlyColor(bool _Set)
	{
		OnlyColor.x = (_Set == true) ? 1.f : 0.f;
	}

public:
	void Start(int _Order = 0);

public:
	GJHSpriteRenderer();
	~GJHSpriteRenderer();

public:
	GJHSpriteRenderer(const GJHSpriteRenderer& _Other) = delete;
	GJHSpriteRenderer(const GJHSpriteRenderer&& _Other) = delete;

public:
	GJHSpriteRenderer operator=(const GJHSpriteRenderer& _Other) = delete;
	GJHSpriteRenderer operator=(const GJHSpriteRenderer&& _Other) = delete;

public:
	void TextureSetting(const GJHGameEngineString& _TexName, bool _AutoSize = false);
	void TextureSetting(std::shared_ptr<GJHDirectTexture2D> _Texture, bool _AutoSize = false);
	void SpriteSetting(const GJHGameEngineString& _SpriteName, int _Index = 0);
	void SpriteSetting(std::shared_ptr<GJHDirectSprite> _Sprite, int _Index = 0);
};