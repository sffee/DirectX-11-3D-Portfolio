#pragma once
#include <list>
#include <map>
#include "GJHGameUpdaterInterface.h"
#include <GJHUpdaterBase.h>
#include <GJHGameEngineMemory.h>
#include <GJHGameEngineString.h>
#include <GJHGameEngineDebug.h>

class GJHGameEngineScene;
class GJHGameEngineActor;
class GJHGameEngineComponent;
class GJHComponentManagement : public GJHGameUpdaterInterface, public GJHUpdaterBase
{
protected:
	GJHGameEngineActor* m_ComMgrActor;
	GJHGameEngineScene* m_ComMgrScene;

private:
	std::list<SharedPtr<GJHGameEngineComponent>> m_Components;
	std::map<GJHGameEngineString, SharedPtr<GJHGameEngineComponent>> m_NameComponents;

public:
	template<typename ComType, typename ... REST>
	SharedPtr<ComType> CreateComponent(REST ... _Arg)
	{
		SharedPtr<ComType> NewCom = std::make_shared<ComType>();
		m_Components.push_back(NewCom);

		NewCom->SetParent(this);
		NewCom->SetScene(m_ComMgrScene);
		NewCom->SetActor(m_ComMgrActor);
		NewCom->Begin();
		NewCom->Start(_Arg...);

		if (m_ComMgrActor != nullptr)
		{
			NewCom->GetActor()->SetClassName(GJHGameEngineString(typeid(ComType).name()));
		}

		return NewCom;
	}

	template<typename ComType>
	SharedPtr<ComType> CreateComponentName(const GJHGameEngineString& _Name)
	{
		SharedPtr<ComType> NewCom = CreateComponent<ComType>();
		NewCom->SetName(_Name);

		m_NameComponents.insert(std::map<GJHGameEngineString, SharedPtr<GJHGameEngineComponent>>::value_type(_Name, NewCom));

		return NewCom;
	}

	template<typename ComType>
	SharedPtr<ComType> FindComponent(const GJHGameEngineString& _Name)
	{
		if (m_NameComponents.find(_Name) == m_NameComponents.end())
		{
			GJHGameEngineDebug::AssertMsg("if (m_NameComponents.find(_Name) == m_NameComponents.end())");
		}

		SharedPtr<ComType> Check = std::dynamic_pointer_cast<ComType, GJHGameEngineComponent>(m_NameComponents[_Name]);
		if (Check == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("SharedPtr<ComType> Check = std::dynamic_pointer_cast<ComType, GJHGameEngineComponent>(m_NameComponents[_Name]);");
		}

		return Check;
	}

	template<typename ComType>
	SharedPtr<ComType> FindComponent()
	{
		for (auto& _Com : m_Components)
		{
			if (_Com->GetType() == typeid(ComType))
			{
				return std::dynamic_pointer_cast<ComType, GJHGameEngineComponent>(_Com);
			}
		}

		return nullptr;
	}

public:
	GJHComponentManagement();
	~GJHComponentManagement();

public:
	GJHComponentManagement(const GJHComponentManagement& _Other) = delete;
	GJHComponentManagement(const GJHComponentManagement&& _Other) = delete;

public:
	GJHComponentManagement operator=(const GJHComponentManagement& _Other) = delete;
	GJHComponentManagement operator=(const GJHComponentManagement&& _Other) = delete;

public:
	void SceneChangeStart() override;
	void SceneChangeEnd() override;

	void UpdatePrev() override;
	void Update() override;
	void UpdateNext() override;
	void CollisionPrev() override;
	void Collision() override;
	void CollisionNext() override;
	void RenderPrev() override;
	void Render() override;
	void DebugRender() override;
	void RenderNext() override;
	void Release() override;
	void DeathTask() override;
	void OnGUI() override;
};