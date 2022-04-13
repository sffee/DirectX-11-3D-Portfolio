#include "GJHLight.h"
#include "GJHRenderManagerComponent.h"
#include "GJHActorTransform.h"
#include <GJHGameEngineScene.h>
#include "GJHCamera.h"
#include <GJHDirectRenderTarget.h>

GJHLightData GJHLight::DefaultData =
{
	float4(1.f, 0.f, 0.f, 0.f),
	float4(1.f, 1.f, 1.f, 1.f),
	float4(1.f, 1.f, 1.f, 1.f),
	float4(1.f, 1.f, 1.f, 1.f),
	float4(0.1f, 0.1f, 0.1f, 1.f),
	float4(1.f, 1.f, 1.f, 1.f), 
	float4(10.f, 0.f, 0.f, 0.f),
	float4(),
	float4(),
	float4(),
	float4(),
	float4(),
	float4x4(),
	float4x4(),
	float4x4(),
	float4x4()
};

GJHLight::GJHLight() :
	Data(DefaultData),
	m_CurCamera(nullptr)
{
}

GJHLight::~GJHLight()
{
}

void GJHLight::CalLightData()
{
	Data.LightPos = GetActorTrans()->GetWorldPosition();
	Data.LightPos = Data.LightPos * m_CurCamera->GetViewMatrix();

	Data.LightDir = GetActorTrans()->GetWorldForward();
	Data.LightDir = Data.LightDir * m_CurCamera->GetViewMatrix();
	Data.LightDirReverse = -Data.LightDir;
	Data.CamPos = m_CurCamera->GetViewCamPos();

	Data.View.View(GetActorTrans()->GetWorldPosition(), GetActorTrans()->GetWorldForward(), GetActorTrans()->GetWorldUp());
	Data.Proj.OrthographicLH((m_ViewSize.x / Data.ShadowRange.w), (m_ViewSize.x / Data.ShadowRange.w), m_Near, m_Far);
	Data.ViewProj = Data.View * Data.Proj;
}

void GJHLight::InitLight(int _Order)
{
	SetOrder(_Order);

	std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
	Ptr->PushLight(this);

	m_Near = 0.1f;
	m_Far = 100000.0f;

	m_ViewSize.x = 4096;
	m_ViewSize.y = 4096;
	Data.ShadowRange.x = 4096;
	Data.ShadowRange.y = 4096;

	Data.ShadowRange.w = 4.0f;

	m_ShadowTarget = std::make_shared<GJHDirectRenderTarget>();
	m_ShadowTarget->CreateRenderTarget({ 4096, 4096 }, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, float4::NONE);
	m_ShadowTarget->CreateDepthTarget({ 4096, 4096 });
}