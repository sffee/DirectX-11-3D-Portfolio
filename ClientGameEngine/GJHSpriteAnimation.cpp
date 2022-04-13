#include "GJHSpriteAnimation.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineTime.h>

GJHSpriteAnimation::SpriteAnimation::SpriteAnimation() :
	m_Renderer(nullptr),
	m_Sprite(nullptr),
	m_IsPieceAnimation(false),
	m_Loop(false),
	m_StartFrame(0),
	m_EndFrame(0),
	m_FrameTime(),
	m_CurTime(1.f),
	m_CurFrame(0),
	m_AnimationEnd(false),
	m_ShowAnimation(true),
	m_AnimationStop(false),
	m_UseTimeScale(true)
{

}

void GJHSpriteAnimation::SpriteAnimation::Reset()
{
	m_CurFrame = m_StartFrame;
	m_CurTime = m_FrameTime[m_StartFrame];
	m_AnimationEnd = false;
	m_ShowAnimation = true;
	m_AnimationStop = false;
}

void GJHSpriteAnimation::SpriteAnimation::Update()
{
	if (m_ShowAnimation == false)
	{
		return;
	}

	if (m_AnimationStop == false)
	{
		m_CurTime -= GJHGameEngineTime::FDeltaTime(1.f, m_UseTimeScale);
	}

	if (m_CurTime <= 0)
	{
		++m_CurFrame;

		if (m_EndFrame < m_CurFrame)
		{
			if (m_Loop == true)
			{
				m_CurFrame = m_StartFrame;
			}
			else
			{
				--m_CurFrame;

				m_AnimationEnd = true;
			}
		}

		m_CurTime = m_FrameTime[m_CurFrame];
	} 

	if (m_IsPieceAnimation == true)
	{
		m_Renderer->TextureSetting(m_Sprite->GetPieceSprite(m_CurFrame));
	}
	else
	{
		m_Renderer->SpriteSetting(m_Sprite, m_CurFrame);
	}
}

std::shared_ptr<GJHSpriteAnimation::SpriteAnimation> GJHSpriteAnimation::FindAnimation(const GJHGameEngineString& _AniName)
{
	if (m_AnimationMap.find(_AniName) == m_AnimationMap.end())
	{
		return nullptr;
	}

	return m_AnimationMap[_AniName];
}

void GJHSpriteAnimation::CreateAnimation(
	const GJHGameEngineString& _AniName,
	const GJHGameEngineString& _SpriteName,
	int _Start,
	int _End,
	float _Time,
	bool _Loop,
	bool _IsPieceAnimation)
{
	if (FindAnimation(_AniName) != nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (FindAni(_AniName) != nullptr)");
	}

	std::shared_ptr<GJHDirectSprite> Sprite = GJHDirectSprite::Find(_SpriteName);

	if (_IsPieceAnimation == true)
	{
		if (_Start < 0 || Sprite->GetPieceSpriteSize() < _Start)
		{
			GJHGameEngineDebug::AssertMsg("if (_Start < 0 || Sprite->GetPieceSpriteSize() <= _Start)");
		}

		if (_End < 0 || Sprite->GetPieceSpriteSize() < _End)
		{
			GJHGameEngineDebug::AssertMsg("if (_End < 0 || Sprite->GetPieceSpriteSize() <= _End)");
		}
	}
	else
	{
		if (_Start < 0 || Sprite->SpriteSize() <= _Start)
		{
			GJHGameEngineDebug::AssertMsg("if (_Start < 0 || Sprite->SpriteSize() <= _Start)");
		}

		if (_End < 0 || Sprite->SpriteSize() <= _End)
		{
			GJHGameEngineDebug::AssertMsg("if (_End < 0 || Sprite->SpriteSize() <= _End)");
		}
	}
	
	std::shared_ptr<SpriteAnimation> NewAni = std::make_shared<SpriteAnimation>();
	NewAni->m_Renderer = m_Render.get();
	NewAni->m_Sprite = Sprite;
	NewAni->m_StartFrame = _Start;
	NewAni->m_EndFrame = _End;
	NewAni->m_Loop = _Loop;
	NewAni->m_IsPieceAnimation = _IsPieceAnimation;
	NewAni->m_AnimationEnd = false;

	for (int i = _Start; i <= _End; ++i)
	{
		NewAni->m_FrameTime[i] = _Time;
	}

	m_AnimationMap[_AniName] = NewAni;
}

void GJHSpriteAnimation::ChangeAnimation(const GJHGameEngineString& _AniName, bool _IsPieceAnimation)
{
	m_CurAnimation = FindAnimation(_AniName);
	m_CurAnimation->Reset();

	if (m_CurAnimation == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_CurAni == nullptr)");
	}

	m_CurAnimation->m_Renderer->TextureSetting(m_CurAnimation->m_Sprite->GetPieceSprite(m_CurAnimation->m_CurFrame));
}

void GJHSpriteAnimation::SetAnimationFrameIndex(int _Index)
{
	if (_Index < m_CurAnimation->m_StartFrame
		|| m_CurAnimation->m_EndFrame < _Index)
	{
		GJHGameEngineDebug::AssertMsg("Animation Index Error");
	}

	m_CurAnimation->m_CurFrame = _Index;
	m_CurAnimation->m_CurTime = m_CurAnimation->m_FrameTime[_Index];
}

int GJHSpriteAnimation::GetAnimationFrameIndex()
{
	return m_CurAnimation->m_CurFrame;
}

bool GJHSpriteAnimation::AnimationEnd()
{
	return m_CurAnimation->m_AnimationEnd;
}

void GJHSpriteAnimation::SetShowAnimation(bool _Set)
{
	m_CurAnimation->m_ShowAnimation = _Set;
}

bool GJHSpriteAnimation::GetShowAnimation()
{
	return m_CurAnimation->m_ShowAnimation;
}

void GJHSpriteAnimation::SetAnimationStop(bool _Set)
{
	m_CurAnimation->m_AnimationStop = _Set;
}

void GJHSpriteAnimation::Reset()
{
	m_CurAnimation->Reset();
}

GJHSpriteAnimation::GJHSpriteAnimation()
{

}

GJHSpriteAnimation::~GJHSpriteAnimation()
{

}

void GJHSpriteAnimation::Start()
{
	m_Render = GetActor()->FindComponent<GJHSpriteRenderer>();

	if (m_Render == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Render == nullptr)");
	}
}

void GJHSpriteAnimation::Update()
{
	if (m_CurAnimation == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_CurAni == nullptr)");
	}

	m_CurAnimation->Update();
}

void GJHSpriteAnimation::SetUseTimeScale(bool _Set)
{
	m_CurAnimation->m_UseTimeScale = _Set;
}

void GJHSpriteAnimation::SetAnimationFrameTime(int _FrameIndex, float _Time)
{
	m_CurAnimation->m_FrameTime[_FrameIndex] = _Time;
}