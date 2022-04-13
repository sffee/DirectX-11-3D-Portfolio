#include "GJHWorldComponent.h"
#include <GJHGameEngineDebug.h>
#include "GJHCollisionComponent.h"

GJHWorldComponent::GJHWorldComponent() :
	m_World()
{

}

GJHWorldComponent::~GJHWorldComponent()
{

}

std::shared_ptr<GJHGameEngineTransform> GJHWorldComponent::PopTransform(bool _IsUpdateCheck)
{
	std::shared_ptr<GJHGameEngineTransform> ReturnTrans = m_World.CreateTrans();

	if (ReturnTrans == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (ReturnTrans == nullptr)");
	}

	return ReturnTrans;
}

void GJHWorldComponent::RenderPrev()
{
	m_World.TransUpdate();
}

void GJHWorldComponent::SetCollisionLink(int _Left, int _Right)
{
	m_World.SetCollisionLink(_Left, _Right);
}

std::shared_ptr<GJHGameEngineCollision> GJHWorldComponent::CreateCollision(int _Order, COLTYPE _Type, GJHGameEngineTransform* _Ptr)
{
	if (_Ptr == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (_Ptr == nullptr)");
	}

	return m_World.CreateCollision(_Order, _Type, *_Ptr);
}

std::list<std::shared_ptr<GJHGameEngineCollision>>& GJHWorldComponent::GetCollisionGroup(int _Order)
{
	return m_World.GetCollisionGroup(_Order);
}

std::list<std::shared_ptr<GJHGameEngineCollision>> GJHWorldComponent::GetCollisionCheck(std::shared_ptr<GJHCollisionComponent> _Origin, float4 MovePos, int _Order)
{
	return m_World.GetCollisionCheck(_Origin->m_Collision, (COLTYPE)_Origin->GetOrder(), MovePos, _Order);
}

void GJHWorldComponent::Collision()
{
	m_World.Collision();
}

void GJHWorldComponent::Release()
{
	m_World.Release();
}
