#include "ObjectBase.h"
#include <GJHFbxRenderer.h>
#include <GJHFbxAnimationRenderer.h>

ObjectBase::ObjectBase() :
	m_Collision(nullptr),
	m_Render(nullptr),
	m_AniRender(nullptr)
{

}

ObjectBase::~ObjectBase()
{

}

void ObjectBase::Update()
{
	CollisionUpdate();
}

void ObjectBase::CollisionUpdate()
{
	if (m_Collision != nullptr)
	{
		if (m_AniRender != nullptr)
		{
			m_Collision->SetLocalRotation(m_AniRender->GetLocalRotation());
		}
		else if (m_Render != nullptr)
		{
			m_Collision->SetLocalRotation(m_Render->GetLocalRotation());
		}
	}
}

void ObjectBase::CreateAnimation(const GJHGameEngineString& _AnimationName, const GJHGameEngineString& _MeshFbxName, const GJHGameEngineString& _AnimationFbxName, int _AnimationIndex, bool _Loop, float _FrameTime)
{
	if (m_AniRender == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Render == nullptr)");
	}

	m_AniRender->CreateUserAnimation(_AnimationName, _MeshFbxName, _AnimationFbxName, L"DefferdFbxAniShader3D", _AnimationIndex, _Loop, _FrameTime);
}

void ObjectBase::ChangeAnimation(const GJHGameEngineString& _AnimationName)
{
	if (m_AniRender == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Render == nullptr)");
	}

	m_AniRender->ChangeAnimation(_AnimationName);
}

bool ObjectBase::AnimationEnd()
{
	if (m_AniRender == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Render == nullptr)");
	}

	return m_AniRender->AnimationEnd();
}