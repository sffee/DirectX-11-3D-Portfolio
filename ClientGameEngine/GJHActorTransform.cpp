#include "GJHActorTransform.h"
#include <GJHGameEngineScene.h>
#include "GJHWorldComponent.h"

GJHActorTransform::GJHActorTransform() :
	m_MasterWorld(nullptr),
	m_WorldTransform()
{

}

GJHActorTransform::~GJHActorTransform()
{

}

void GJHActorTransform::CalMatrixOn()
{
	m_WorldTransform->CalMatrixOn();
}
void GJHActorTransform::CalMatrixOff()
{
	m_WorldTransform->CalMatrixOff();
}

void GJHActorTransform::Start()
{
	std::shared_ptr<GJHWorldComponent> Ptr = GetScene()->FindComponent<GJHWorldComponent>(L"World");

	m_WorldTransform = Ptr->PopTransform();

	m_WorldTransform->SetParent(this);

	m_MasterWorld = Ptr.get();
}

void GJHActorTransform::CalRenderDataCopy()
{
	if (m_WorldTransform->IsCalMatrix() == false)
	{
		return;
	}

	memcpy_s(&m_RenderingData, sizeof(m_RenderingData), &m_WorldTransform->TransData, sizeof(m_WorldTransform->TransData));
}

void GJHActorTransform::CameraSetting(GJHCamera* _Camera)
{
	m_RenderingData.CalRenderingMatrix();
}

void GJHActorTransform::DeathTask()
{
	if (m_WorldTransform == nullptr)
	{
		m_WorldTransform->Death();
		m_WorldTransform = nullptr;
	}
}