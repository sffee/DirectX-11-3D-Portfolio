#include "GJHCamera.h"
#include "GJHActorTransform.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineScene.h>
#include "GJHRenderManagerComponent.h"
#include <GJHDirectRenderTarget.h>
#include <GJHGameEngineWindow.h>
#include "GJHDefferdLighting.h"
#include "GJHRenderer.h"
#include "GJHLight.h"
#include <GJHDirectStructuredBuffer.h>

GJHCamera::GJHCamera() :
	m_Near(0.1f),
	m_Far(100000.f),
	m_Fov(60.f),
	m_ProjectionSize(100.f, 100.f),
	m_Trans(nullptr),
	m_Mode(CamProjectionMode::Perspective)
{
}

GJHCamera::~GJHCamera()
{

}

void GJHCamera::InitCam(int _Order)
{
	SetOrder(_Order);

	std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
	Ptr->PushCamera(this);

	std::shared_ptr<GJHActorTransform> TransForm = GetActor()->FindComponent<GJHActorTransform>();
	m_Trans = TransForm.get();

	m_CameraTarget = std::make_shared<GJHDirectRenderTarget>();
	m_CameraTarget->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
	m_CameraTarget->SetDepthTarget(GJHGameEngineDevice::GetBackBuffer()->GetDepthTarget());

	for (OutPutPathType _Type : m_PathList)
	{
		PathTargetCreate(_Type);
	}
}

void GJHCamera::PathTargetCreate(OutPutPathType Type)
{
	switch (Type)
	{
	case OutPutPathType::Forward:
	{
		m_CameraPathTarget[OutPutPathType::Forward] = std::make_shared<GJHDirectRenderTarget>();
		m_CameraPathTarget[OutPutPathType::Forward]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_CameraPathTarget[OutPutPathType::Forward]->SetDepthTarget(GJHGameEngineDevice::GetBackBuffer()->GetDepthTarget());
		break;
	}
	case OutPutPathType::Defferd:
		m_CameraPathTarget[OutPutPathType::Defferd] = std::make_shared<GJHDirectRenderTarget>();
		
		m_CameraPathTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_CameraPathTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_CameraPathTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_CameraPathTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_CameraPathTarget[OutPutPathType::Defferd]->SetDepthTarget(GJHGameEngineDevice::GetBackBuffer()->GetDepthTarget());

		m_LightTarget[OutPutPathType::Defferd] = std::make_shared<GJHDirectRenderTarget>();

		m_LightTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_LightTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);
		m_LightTarget[OutPutPathType::Defferd]->CreateRenderTarget(GJHGameEngineWindow::GetSize(), float4::NONE);

		m_DefferdTargetPostEffect = std::make_shared<GJHDefferdLighting>();
		m_DefferdTargetPostEffect->SetEffectTarget(m_CameraPathTarget[OutPutPathType::Defferd]);
		m_DefferdTargetPostEffect->SetResultTarget(m_LightTarget[OutPutPathType::Defferd]);
		m_DefferdTargetPostEffect->Start();

		break;
	default:
		break;
	}
}

void GJHCamera::RenderPrev()
{
	CalCameraMatrix();
}

void GJHCamera::CalCameraMatrix()
{
	m_Trans->m_RenderingData.View.View(m_Trans->GetWorldPosition(), m_Trans->GetWorldForward(), m_Trans->GetWorldUp());

	switch (m_Mode)
	{
	case CamProjectionMode::Orthographic:
		m_Trans->m_RenderingData.Projection.OrthographicLH(m_ProjectionSize.x, m_ProjectionSize.y, m_Near, m_Far);
		break;
	case CamProjectionMode::Perspective:
		m_Trans->m_RenderingData.Projection.PerspectiveFovDegLH(m_Fov, m_ProjectionSize.x, m_ProjectionSize.y, m_Near, m_Far);
		break;
	default:
		break;
	}
}

float4 GJHCamera::GetViewCamPos()
{
	return GetActorTrans()->GetWorldPosition() * m_Trans->m_RenderingData.View;
}

float4x4 GJHCamera::GetViewMatrix()
{
	return m_Trans->m_RenderingData.View;
}

void GJHCamera::CamTargetMerge()
{
	m_CameraTarget->Merge(m_CameraPathTarget[OutPutPathType::Forward]);

	if (m_DefferdTargetPostEffect != nullptr)
	{
		m_DefferdTargetPostEffect->DefferdMerge();
		m_CameraTarget->Merge(m_DefferdTargetPostEffect->GetDefferdTarget());
	}
}

void GJHCamera::CamTargetClear()
{
	for (auto& PathTarget : m_CameraPathTarget)
	{
		if (PathTarget.second == nullptr)
		{
			continue;
		}

		PathTarget.second->Clear();
	}

	m_CameraTarget->Clear();
}

bool GJHCamera::CamPathTargetSetting(OutPutPathType _Path)
{
	if (m_CameraPathTarget.find(_Path) == m_CameraPathTarget.end())
	{
		return false;
	}

	if (m_CameraPathTarget[_Path] == nullptr)
	{
		return false;
	}

	m_CameraPathTarget[_Path]->Setting();

	return true;
}

void GJHCamera::CamTargetSetting()
{
	m_CameraTarget->Setting();
}

void GJHCamera::DefferdLightTargetClear()
{
	m_DefferdTargetPostEffect->GetResultTarget()->Clear();
}

void GJHCamera::DefferdLightTargetProcess(OutPutPathType _OutPutType)
{
	if (_OutPutType == OutPutPathType::Defferd)
	{
		CurDefferdLight->GetLightData().CamInverse = m_Trans->m_RenderingData.View.InverseReturn();

		m_DefferdTargetPostEffect->GetLightingPlayer().TextureSetting("Shadow", CurDefferdLight->GetShadowTarget()->GetTex(0));
		m_DefferdTargetPostEffect->GetLightingPlayer().CBufferSettingLink("OneLightData", CurDefferdLight->GetLightData());

		m_DefferdTargetPostEffect->Effect();
	}
}