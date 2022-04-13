#include "ObjectPre.h"
#include "GJHComponentManagement.h"
#include "GJHGameEngineComponent.h"
#include <GJHUpdaterBase.h>

GJHComponentManagement::GJHComponentManagement()
{

}

GJHComponentManagement::~GJHComponentManagement()
{

}

void GJHComponentManagement::SceneChangeStart()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->SceneChangeStart();
	}
}

void GJHComponentManagement::SceneChangeEnd()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->SceneChangeEnd();
	}
}

void GJHComponentManagement::UpdatePrev()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->UpdatePrev();
	}
}

void GJHComponentManagement::Update()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->Update();
	}
}

void GJHComponentManagement::UpdateNext()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->UpdateNext();
	}
}

void GJHComponentManagement::CollisionPrev()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->CollisionPrev();
	}
}

void GJHComponentManagement::Collision()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->Collision();
	}
}

void GJHComponentManagement::CollisionNext()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->CollisionNext();
	}
}

void GJHComponentManagement::RenderPrev()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->RenderPrev();
	}
}

void GJHComponentManagement::Render()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->Render();
	}
}

void GJHComponentManagement::DebugRender()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->DebugRender();
	}
}

void GJHComponentManagement::RenderNext()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->RenderNext();
	}
}

void GJHComponentManagement::Release()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->Release();
	}
	
	GJHUpdaterBase::ReleaseList(m_Components);
}

void GJHComponentManagement::DeathTask()
{
	for (auto& _Com : m_Components)
	{
		_Com->DeathTask();
	}
}

void GJHComponentManagement::OnGUI()
{
	for (auto& _Com : m_Components)
	{
		if (_Com->IsUpdate() == false)
		{
			continue;
		}

		_Com->OnGUI();
	}
}