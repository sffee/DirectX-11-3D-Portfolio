#include "GJHMaskRenderer.h"
#include <GJHDirectSprite.h>
#include <GJHGameEngineActor.h>
#include <GJHGameEngineScene.h>
#include "GJHRenderManagerComponent.h"
#include "GJHCamera.h"

GJHMaskRenderer::GJHMaskRenderer()
{

}

GJHMaskRenderer::~GJHMaskRenderer()
{

}

void GJHMaskRenderer::Start(int _Order)
{
	SetOrder(_Order);

	GJHActorTransform::Start();

	ActorTransPtr Trans = GetActor()->FindComponent<GJHActorTransform>();

	if (Trans == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Trans == nullptr)");
	}

	this->AttachParent(Trans);

	std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");

	Ptr->PushRenderer(this);
}

void GJHMaskRenderer::CreateTexture2DMaskPlayer(const GJHGameEngineString& _TextureName, int _TargetIndex, float4 _Color)
{
	CreateRenderPlayer(L"Texture2DMaskMat", L"Rect");
	CBufferSettingNew(L"MaskColor", _Color);
	float4 CutData = { 0.f, 0.f, 1.f, 1.f };
	CBufferSettingNew(L"SpriteCutData", CutData);
	TextureSetting(L"Tex", _TextureName);

	std::shared_ptr<GJHDirectRenderTarget> m_RenderTarget = GJHRenderManagerComponent::GetMask(_TargetIndex);

	if (m_RenderTarget == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_RenderTarget == nullptr)");
	}

	m_Target.push_back(m_RenderTarget);
}

void GJHMaskRenderer::Rendering()
{
	GJHRenderingTransformData& _CameraData = m_CurCamera->GetCameraTransformMatrix();
	CalRenderDataCopy();
	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = _CameraData.View;
	RendererData.Projection = _CameraData.Projection;
	RendererData.CalRenderingMatrix();

	for (size_t i = 0; i < m_PlayerList.size(); i++)
	{
		m_Target[i]->Setting();
		m_PlayerList[i]->Render();
	}
}