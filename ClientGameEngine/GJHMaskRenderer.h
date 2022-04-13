#pragma once
#include "GJHRenderer.h"

class GJHDirectSprite;
class GJHRenderManagerComponent;
class GJHMaskRenderer : public GJHRenderer
{
private:
	friend GJHRenderManagerComponent;

	enum MASKCOLORTYPE
	{
		PLAYERCOLOR,
		ONECOLOR
	};

	class GJHRenderer;
	class MaskInfo
	{
	public:
		float4 Color;
		std::shared_ptr<GJHCamera> m_Cam;
		std::shared_ptr<GJHRenderPlayer> m_Player;
		std::shared_ptr<GJHDirectRenderTarget> m_MaskTarget;
	};

public:
	std::vector<std::shared_ptr<GJHDirectRenderTarget>> m_Target;

public:
	void Start(int _Order = 0);

public:
	GJHMaskRenderer();
	~GJHMaskRenderer();

public:
	GJHMaskRenderer(const GJHMaskRenderer& _Other) = delete;
	GJHMaskRenderer(const GJHMaskRenderer&& _Other) = delete;

public:
	GJHMaskRenderer operator=(const GJHMaskRenderer& _Other) = delete;
	GJHMaskRenderer operator=(const GJHMaskRenderer&& _Other) = delete;

private:
	void Rendering();

public:
	void CreateTexture2DMaskPlayer(const GJHGameEngineString& _TextureName, int _TargetIndex, float4 _Color);
};