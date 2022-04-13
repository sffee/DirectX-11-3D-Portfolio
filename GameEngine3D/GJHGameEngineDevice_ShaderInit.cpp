#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHDirectResourcesManager.h"
#include "GJHVertexHeader.h"
#include "GJHDirectVertexShader.h"
#include "GJHDirectGeometryShader.h"
#include "GJHDirectPixelShader.h"
#include <GJHGameEngineDirectory.h>

void MaskShader(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"2DTextureMaskShader.hlsl"), L"TexMaskVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"2DTextureMaskShader.hlsl"), L"TexMaskPixShader");
	}
}

void PostEffectShader(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdMerge.hlsl"), L"DefferdMergeVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdMerge.hlsl"), L"DefferdMergePix");
	}


	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdLighting.hlsl"), L"DefferdLightingVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdLighting.hlsl"), L"DefferdLightingPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Uvdistortion.hlsl"), L"UvdistortionVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Uvdistortion.hlsl"), L"UvdistortionPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Grid2D.hlsl"), L"Grid2DVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Grid2D.hlsl"), L"Grid2DPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"OutLine.hlsl"), L"OutLineVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"OutLine.hlsl"), L"OutLinePix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"MaskCulling.hlsl"), L"MaskCullingVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"MaskCulling.hlsl"), L"MaskCullingPix");
	}
}

void Shader2D(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"ColorShader.hlsl"), L"VtxColorShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"ColorShader.hlsl"), L"PixColorShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"2DTextureShader.hlsl"), L"TexVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"2DTextureShader.hlsl"), L"TexPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"CircleShader.hlsl"), L"CircleVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"CircleShader.hlsl"), L"CirclePixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"BoxShader.hlsl"), L"BoxVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"BoxShader.hlsl"), L"BoxPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"SceneChangeStartShader.hlsl"), L"TexVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"SceneChangeStartShader.hlsl"), L"TexPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"SceneChangeEndShader.hlsl"), L"TexVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"SceneChangeEndShader.hlsl"), L"TexPixShader");
	}
}

void Shader3D(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"ShadowDepth.hlsl"), L"ShadowDepthVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"ShadowDepth.hlsl"), L"ShadowDepthPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdHeightMapShader.hlsl"), L"DefferdHeightMapVtxShader");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdHeightMapShader.hlsl"), L"DefferdHeightMapPixShader");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdStatic3DMeshTexture.hlsl"), L"DefferdStatic3DMeshTextureVtx");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdStatic3DMeshTexture.hlsl"), L"DefferdStatic3DMeshTexturePix");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdStatic3DMeshColor.hlsl"), L"DefferdStatic3DMeshColorVtx");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdStatic3DMeshColor.hlsl"), L"DefferdStatic3DMeshColorPix");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdFbxAniColorShader3D.hlsl"), L"DefferdFbxAniColorShader3DVtx");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdFbxAniColorShader3D.hlsl"), L"DefferdFbxAniColorShader3DPix");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdFbxAniShader3D.hlsl"), L"DefferdFbxAniShader3DVtx");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdFbxAniShader3D.hlsl"), L"DefferdFbxAniShader3DPix");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"DefferdFbxShader3D.hlsl"), L"DefferdFbxShader3DVtx");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"DefferdFbxShader3D.hlsl"), L"DefferdFbxShader3DPix");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"ShadowAniDepth.hlsl"), L"ShadowAniDepthVtx");
		std::shared_ptr<GJHDirectPixelShader> Pix = GJHDirectPixelShader::Create(Dir.PlusFilePath(L"ShadowAniDepth.hlsl"), L"ShadowAniDepthPix");
		Pix->SetOutPutPath(OutPutPathType::Defferd);
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Static3DMeshColor.hlsl"), L"Static3DMeshColorVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Static3DMeshColor.hlsl"), L"Static3DMeshColorPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"3DMeshColor.hlsl"), L"MeshColorVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"3DMeshColor.hlsl"), L"MeshColorPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"HeightMapShader.hlsl"), L"HeightMapVtxShader");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"HeightMapShader.hlsl"), L"HeightMapPixShader");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"FbxAniShader3D.hlsl"), L"FbxAniShader3DVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"FbxAniShader3D.hlsl"), L"FbxAniShader3DPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"TextureShader3D.hlsl"), L"TextureShader3DVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"TextureShader3D.hlsl"), L"TextureShader3DPix");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"Particle.hlsl"), L"VS_Particle");
		GJHDirectGeometryShader::Create(Dir.PlusFilePath(L"Particle.hlsl"), L"GS_Particle");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"Particle.hlsl"), L"PS_Particle");
	}

	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"EngineGrid3D.hlsl"), L"EngineGrid3DVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"EngineGrid3D.hlsl"), L"EngineGrid3DPix");
	}
}

void TargetShader(GJHGameEngineDirectory& Dir)
{
	{
		GJHDirectVertexShader::Create(Dir.PlusFilePath(L"TargetMergeShader.hlsl"), L"MergeVtx");
		GJHDirectPixelShader::Create(Dir.PlusFilePath(L"TargetMergeShader.hlsl"), L"MergePix");
	}
}

void GJHGameEngineDevice::ShaderInit()
{
	GJHGameEngineDirectory Dir;
	Dir.MoveParent(L"GJH");
	Dir.Move(L"ShaderProject");

	Shader3D(Dir);
	TargetShader(Dir);
	MaskShader(Dir);
	Shader2D(Dir);
	PostEffectShader(Dir);
}