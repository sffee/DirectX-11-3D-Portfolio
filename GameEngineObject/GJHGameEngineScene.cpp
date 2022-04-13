#include "ObjectPre.h"
#include "GJHGameEngineScene.h"
#include "GJHGameEngineActor.h"

void(__stdcall* GJHGameEngineScene::m_ActorCreateFunc)(ActorPtr& _Actor) = nullptr;

GJHGameEngineScene::GJHGameEngineScene()
{
	m_ComMgrScene = this;
}

GJHGameEngineScene::~GJHGameEngineScene()
{

}

void GJHGameEngineScene::SceneChangeStart()
{
	GJHComponentManagement::SceneChangeStart();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->SceneChangeStart();
	}
}

void GJHGameEngineScene::SceneChangeEnd()
{
	GJHComponentManagement::SceneChangeEnd();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->SceneChangeEnd();
	}
}

void GJHGameEngineScene::UpdatePrev()
{
	GJHComponentManagement::UpdatePrev();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->UpdatePrev();
	}
}

void GJHGameEngineScene::Update()
{
	GJHComponentManagement::Update();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->Update();
	}
}

void GJHGameEngineScene::UpdateNext()
{
	GJHComponentManagement::UpdateNext();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->UpdateNext();
	}
}

void GJHGameEngineScene::CollisionPrev()
{
	GJHComponentManagement::CollisionPrev();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->CollisionPrev();
	}
}

void GJHGameEngineScene::Collision()
{
	GJHComponentManagement::Collision();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->Collision();
	}
}

void GJHGameEngineScene::CollisionNext()
{
	GJHComponentManagement::CollisionNext();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->CollisionNext();
	}
}

void GJHGameEngineScene::RenderPrev()
{
	GJHComponentManagement::RenderPrev();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->RenderPrev();
	}
}

void GJHGameEngineScene::Render()
{
	GJHComponentManagement::Render();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->Render();
	}
}

void GJHGameEngineScene::DebugRender()
{
	GJHComponentManagement::DebugRender();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->DebugRender();
	}
}

void GJHGameEngineScene::RenderNext()
{
	GJHComponentManagement::RenderNext();

	for (auto& _Actor : m_Actors)
	{
		if (_Actor->IsUpdate() == false)
		{
			continue;
		}

		_Actor->RenderNext();
	}
}

void GJHGameEngineScene::Release()
{
	GJHComponentManagement::Release();

	GJHUpdaterBase::ReleaseList(m_Actors);
}

void GJHGameEngineScene::OnGUI()
{
	GJHComponentManagement::OnGUI();
}

std::shared_ptr<GJHGameEngineActor> GJHGameEngineScene::CreateActor()
{
	ActorPtr Ptr = std::make_shared<GJHGameEngineActor>();
	Ptr->SetScene(this);
	Ptr->m_ComMgrScene = this;
	m_Actors.push_back(Ptr);

	if (m_ActorCreateFunc != nullptr)
	{
		m_ActorCreateFunc(Ptr);
	}

	return Ptr;
}

std::list<ActorPtr>& GJHGameEngineScene::GetActorList()
{
	return m_Actors;
}