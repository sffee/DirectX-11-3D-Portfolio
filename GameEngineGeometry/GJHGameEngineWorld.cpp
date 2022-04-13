#include "GeoPre.h"
#include "GJHGameEngineWorld.h"
#include "GJHGameEngineTransform.h"
#include "GJHGameEngineCollision.h"
#include <algorithm>

GJHGameEngineWorld::GJHGameEngineWorld() :
	m_bParentTransCheck(true),
	m_TransPool(this)
{

}

GJHGameEngineWorld::~GJHGameEngineWorld()
{

}

SharedPtr<GJHGameEngineTransform> GJHGameEngineWorld::CreateTrans(bool _ActiveList)
{
	SharedPtr<GJHGameEngineTransform> NewTrans = m_TransPool.PopObject();

	if (NewTrans == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (NewTrans == nullptr)");
	}
	else
	{
		NewTrans->Reset();
	}

	if (_ActiveList == true)
	{
		m_ActiveList.push_back(NewTrans); 
	}

	return NewTrans;
}

void GJHGameEngineWorld::PushTrans(SharedPtr<GJHGameEngineTransform> _Trans) 
{
	// 부모가 있으니 루트로 갈수는 없다.
	if (nullptr != _Trans->GetParent<GJHGameEngineTransform>())
	{
		return;
	}

	m_ActiveList.push_back(_Trans);
}

void GJHGameEngineWorld::TransUpdate()
{
	m_TransPool.ObjectMemoryCheck();

	if (m_bParentTransCheck == true)
	{
		std::list<std::shared_ptr<GJHGameEngineTransform>>::iterator Start = m_ActiveList.begin();
		std::list<std::shared_ptr<GJHGameEngineTransform>>::iterator End = m_ActiveList.end();

		for (; Start != End; )
		{
			if ((*Start)->IsParent() == true)
			{
				Start = m_ActiveList.erase(Start);
			}
			else
			{
				++Start;
			}
		}

		m_bParentTransCheck = false;
	}

	for (auto& _Transform : m_ActiveList)
	{
		if (_Transform->IsUpdate() == false)
		{
			continue;
		}

		_Transform->CalTransData();
	}
}

void GJHGameEngineWorld::Release()
{
	for (auto& _ColList : m_ColGroup)
	{
		std::list<std::shared_ptr<GJHGameEngineCollision>>::iterator Start = _ColList.second.begin();
		std::list<std::shared_ptr<GJHGameEngineCollision>>::iterator End = _ColList.second.end();

		for (; Start != End; )
		{
			if ((*Start)->IsDeath() == true)
			{
				(*Start)->DeathTask();
				Start = _ColList.second.erase(Start);
			}
			else
			{
				++Start;
			}
		}
	}

	std::list<std::shared_ptr<GJHGameEngineTransform>>::iterator Start = m_ActiveList.begin();
	std::list<std::shared_ptr<GJHGameEngineTransform>>::iterator End = m_ActiveList.end();

	for (; Start != End; )
	{
		if ((*Start)->IsDeath() == true)
		{
			(*Start)->DeathTask();
			Start = m_ActiveList.erase(Start);
		}
		else
		{
			++Start;
		}
	}

	m_TransPool.ObjectMemoryCheck();
}

void GJHGameEngineWorld::SetCollisionLink(int _Left, int _Right)
{
	if (m_ColLink.find(_Left) == m_ColLink.end())
	{
		m_ColLink[_Left] = std::set<int>();
	}

	if (m_ColLink[_Left].find(_Right) != m_ColLink[_Left].end())
	{
		return;
	}

	if (m_ColUpdateSet.find(_Left) == m_ColUpdateSet.end())
	{
		m_ColUpdateSet.insert(_Left);
	}

	if (m_ColUpdateSet.find(_Right) == m_ColUpdateSet.end())
	{
		m_ColUpdateSet.insert(_Right);
	}

	m_ColLink[_Left].insert(_Right);
}

void GJHGameEngineWorld::Collision()
{
	if (m_ColLink.size() == 0)
	{
		return;
	}

	for (auto& _Var : m_ColUpdateSet)
	{
		if (m_ColGroup.find(_Var) == m_ColGroup.end())
		{
			continue;
		}

		std::list<std::shared_ptr<GJHGameEngineCollision>>& UpdateGroup = m_ColGroup[_Var];

		for (auto& UpdateCol : UpdateGroup)
		{
			UpdateCol->ColUpdate();
		}
	}

	for (auto& _Var : m_ColLink)
	{
		const int& ThisLayer = _Var.first;
		std::set<int>& OtherGroup = _Var.second;

		for (auto& OtherLayer : OtherGroup)
		{
			std::list<std::shared_ptr<GJHGameEngineCollision>>& ColThisGroup = m_ColGroup[ThisLayer];

			if (ThisLayer == OtherLayer)
			{

			}
			else
			{
				std::list<std::shared_ptr<GJHGameEngineCollision>>& ColOtherGroup = m_ColGroup[OtherLayer];

				for (std::shared_ptr<GJHGameEngineCollision>& ThisCol : ColThisGroup)
				{
					for (std::shared_ptr<GJHGameEngineCollision>& OtherCol : ColOtherGroup)
					{
						if (ThisCol->IsUpdate() == false
							|| OtherCol->IsUpdate() == false)
						{
							continue;
						}

						ThisCol->ColCheck(*OtherCol);
					}
				}
			}
		}
	}
}

std::shared_ptr<GJHGameEngineCollision> GJHGameEngineWorld::CreateCollision(int _Order, COLTYPE _Type, GJHGameEngineTransform& _Transform)
{
	std::shared_ptr<GJHGameEngineCollision> Ptr = std::make_shared<GJHGameEngineCollision>(_Transform);
	Ptr->SetOrder(_Order);
	Ptr->SetType(_Type);

	if (m_ColGroup.find(_Order) == m_ColGroup.end())
	{
		m_ColGroup[_Order] = std::list<std::shared_ptr<GJHGameEngineCollision>>();
	}

	m_ColGroup[_Order].push_back(Ptr);

	return Ptr;
}

std::list<std::shared_ptr<GJHGameEngineCollision>>& GJHGameEngineWorld::GetCollisionGroup(int _Order)
{
	return m_ColGroup[_Order];
}

std::list<std::shared_ptr<GJHGameEngineCollision>> GJHGameEngineWorld::GetCollisionCheck(std::shared_ptr<GJHGameEngineCollision> _Collision, COLTYPE _Coltype, float4 MovePos, int _Order)
{
	std::list<std::shared_ptr<GJHGameEngineCollision>> ColList;

	auto& Group = GetCollisionGroup(_Order);

	GJHGameEngineTransform Trans;

	Trans.ColUpdate(_Coltype, _Collision->GetTransform().GetWorldPosition() + MovePos
		, _Collision->GetTransform().GetWorldScale()
		, _Collision->GetTransform().GetWorldRotation());
	

	for (std::shared_ptr<GJHGameEngineCollision>& OtherCol : Group)
	{
		if (OtherCol->IsUpdate() == false)
		{
			continue;
		}

		if (true == OtherCol->ColCheck(Trans, _Coltype))
		{
			ColList.push_back(OtherCol);
		}
	}

	return ColList;
}