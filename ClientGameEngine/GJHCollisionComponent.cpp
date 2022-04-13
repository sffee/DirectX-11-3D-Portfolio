#include "GJHCollisionComponent.h"
#include <GJHGameEngineActor.h>
#include "GJHWorldComponent.h"
#include "GJHRenderer.h"
#include "GJHSpriteRenderer.h"

GJHCollisionComponent::GJHCollisionComponent() :
	m_Collision(nullptr),
	m_DebugRender(nullptr)
{

}

GJHCollisionComponent::~GJHCollisionComponent()
{

}

void GJHCollisionComponent::Start(int _Order, COLTYPE _Type)
{
	GJHActorTransform::Start();

	ActorTransPtr Trans = GetActor()->FindComponent<GJHActorTransform>();

	if (Trans == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Trans == nullptr)");
	}

	this->AttachParent(Trans);

	std::shared_ptr<GJHGameEngineCollision> Ptr = GetMasterWorld()->CreateCollision(_Order, _Type, this->GetWorldTransform());
	m_Collision = Ptr;
	m_Collision->SetParent(this);

	m_Collision->SetData(this);
	m_Collision->ColTrueEvent(&GJHCollisionComponent::ColTrue, *this);
	m_Collision->ColFalseEvent(&GJHCollisionComponent::ColFalse, *this);

	m_DebugRender = GetActor()->CreateComponent<GJHRenderer>(10);
	m_DebugRender->CreateRenderPlayer(L"DebugBox", L"Box");
	m_DebugRender->CBufferSettingLink(L"OutPutColor", float4::GREEN);
}

void GJHCollisionComponent::CallEnter(GJHCollisionComponent* _Other)
{
	for (size_t i = 0; i < m_EnterList.size(); i++)
	{
		m_EnterList[i](_Other);
	}
}

void GJHCollisionComponent::CallStay(GJHCollisionComponent* _Other)
{
	for (size_t i = 0; i < m_StayList.size(); i++)
	{
		m_StayList[i](_Other);
	}
}

void GJHCollisionComponent::CallExit(GJHCollisionComponent* _Other)
{
	for (size_t i = 0; i < m_ExitList.size(); i++)
	{
		m_ExitList[i](_Other);
	}
}

void GJHCollisionComponent::ColTrue(GJHGameEngineCollision* _Other)
{
	GJHCollisionComponent* OtherCol = _Other->GetData<GJHCollisionComponent>();

	if (OtherCol == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (OtherCol == nullptr)");
	}

	if (m_OtherCol.find(OtherCol) == m_OtherCol.end())
	{
		m_OtherCol.insert(OtherCol);
		OtherCol->m_OtherCol.insert(this);
		CallEnter(OtherCol);
		OtherCol->CallEnter(this);
	}
	else
	{
		CallStay(OtherCol);
		OtherCol->CallStay(this);
	}
}

void GJHCollisionComponent::ColFalse(GJHGameEngineCollision* _Other)
{
	GJHCollisionComponent* OtherCol = _Other->GetData<GJHCollisionComponent>();

	if (OtherCol == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (OtherCol == nullptr)");
	}

	if (m_OtherCol.find(OtherCol) != m_OtherCol.end())
	{
		CallExit(OtherCol);
		OtherCol->CallExit(this);
		m_OtherCol.erase(OtherCol);
		OtherCol->m_OtherCol.erase(this);
	}
}

int GJHCollisionComponent::GetOrder()
{
	return m_Collision->GetOrder();
}

void GJHCollisionComponent::SetLocalPosition(const float4& _Pos)
{
	GJHActorTransform::SetLocalPosition(_Pos);
	m_DebugRender->SetLocalPosition(_Pos);
}

void GJHCollisionComponent::SetLocalMove(const float4& _Pos)
{
	GJHActorTransform::SetLocalMove(_Pos);
	m_DebugRender->SetLocalMove(_Pos);
}

void GJHCollisionComponent::SetLocalScale(const float4& _Scale)
{
	GJHActorTransform::SetLocalScale(_Scale);
	m_DebugRender->SetLocalScale(_Scale);
}

void GJHCollisionComponent::SetLocalRotation(const float4& _Rotation)
{
	GJHActorTransform::SetLocalRotation(_Rotation);
	m_DebugRender->SetLocalRotation(_Rotation);
}