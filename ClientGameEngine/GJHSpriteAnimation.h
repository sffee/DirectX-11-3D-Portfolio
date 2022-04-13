#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHDirectSprite.h>
#include "GJHRenderer.h"
#include <vector>
#include "GJHSpriteRenderer.h"

class GJHSpriteAnimation : public GJHGameEngineComponent
{
private:
	class SpriteAnimation : std::enable_shared_from_this<GJHSpriteAnimation>
	{
	public:
		GJHSpriteRenderer* m_Renderer;
		std::shared_ptr<GJHDirectSprite> m_Sprite;
		bool m_IsPieceAnimation;
		bool m_Loop;
		int m_StartFrame;
		int m_EndFrame;
		std::map<int, float> m_FrameTime;

		int m_CurFrame;
		float m_CurTime;

		bool m_AnimationEnd;
		bool m_ShowAnimation;
		bool m_AnimationStop;

		bool m_UseTimeScale;

	public:
		SpriteAnimation();

	public:
		void Reset();
		void Update();
	};

private:
	std::shared_ptr<SpriteAnimation> FindAnimation(const GJHGameEngineString& _AniName);

public:
	void CreateAnimation(const GJHGameEngineString& _AniName, const GJHGameEngineString& _SpriteName, int _Start, int _End, float _Time = 0.1f, bool _Loop = false, bool _IsPiece = true);

private:
	std::map<GJHGameEngineString, std::shared_ptr<SpriteAnimation>> m_AnimationMap;
	std::shared_ptr<SpriteAnimation> m_CurAnimation;
	std::shared_ptr<GJHSpriteRenderer> m_Render;

public:
	void ChangeAnimation(const GJHGameEngineString& _AniName, bool _IsPieceAnimation = true);
	void SetAnimationFrameIndex(int _Index);
	int GetAnimationFrameIndex();
	bool AnimationEnd();
	void SetShowAnimation(bool _Set);
	bool GetShowAnimation();
	void SetAnimationStop(bool _Set);
	void SetUseTimeScale(bool _Set);
	void Reset();
	void SetAnimationFrameTime(int _FrameIndex, float _Time);

public:
	GJHSpriteAnimation();
	~GJHSpriteAnimation();

public:
	GJHSpriteAnimation(const GJHSpriteAnimation& _Other) = delete;
	GJHSpriteAnimation(const GJHSpriteAnimation&& _Other) = delete;

public:
	GJHSpriteAnimation operator=(const GJHSpriteAnimation& _Other) = delete;
	GJHSpriteAnimation operator=(const GJHSpriteAnimation&& _Other) = delete;

public:
	void Start() override;
	void Update() override;
};

using AnimationPtr = std::shared_ptr<GJHSpriteAnimation>;