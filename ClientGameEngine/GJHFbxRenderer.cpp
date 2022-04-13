#include "GJHFbxRenderer.h"
#include "GJHCamera.h"

GJHFbxRenderer::GJHFbxRenderer()
{
}

GJHFbxRenderer::~GJHFbxRenderer()
{
}

void GJHFbxRenderer::SetMesh(const GJHGameEngineString& _MeshName)
{
	std::shared_ptr<GJHDirectFbx> MeshFbx = GJHDirectFbx::Find(_MeshName);

	if (MeshFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (MeshFbx == nullptr)");
		return;
	}

	SetFbxToCreateAllRenderPlayer(_MeshName, L"DefferdStatic3DMeshTexture", GetOrder(), nullptr);
}

void GJHFbxRenderer::RenderingSetting()
{
	GJHRenderingTransformData& _CameraData = m_CurCamera->GetCameraTransformMatrix();
	CalRenderDataCopy();
	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = _CameraData.View;
	RendererData.Projection = _CameraData.Projection;
	RendererData.CalRenderingMatrix();
}