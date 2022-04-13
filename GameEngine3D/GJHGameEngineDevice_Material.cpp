#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHMaterial.h"

void GJHGameEngineDevice::MaterialInit()
{
	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"ShadowAniDepth");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"ShadowAniDepth.hlsl");
		Mat->SetPixelShader(L"ShadowAniDepth.hlsl");
	}


	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdFbxAniColorShader3D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"DefferdFbxAniColorShader3D.hlsl");
		Mat->SetPixelShader(L"DefferdFbxAniColorShader3D.hlsl");
	}


	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdFbxAniShader3D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"DefferdFbxAniShader3D.hlsl");
		Mat->SetPixelShader(L"DefferdFbxAniShader3D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdFbxShader3D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"DefferdFbxShader3D.hlsl");
		Mat->SetPixelShader(L"DefferdFbxShader3D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"ShadowDepth");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"ShadowDepth.hlsl");
		Mat->SetPixelShader(L"ShadowDepth.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdMerge");
		Mat->SetBlend(L"DefferdBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"DefferdMerge.hlsl");
		Mat->SetPixelShader(L"DefferdMerge.hlsl");
	}


	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdLighting");
		Mat->SetBlend(L"DefferdBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"DefferdLighting.hlsl");
		Mat->SetPixelShader(L"DefferdLighting.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"TargetCopyMaterial");
		Mat->SetBlend(L"TargetMergeBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"TargetMergeShader.hlsl");
		Mat->SetPixelShader(L"TargetMergeShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Grid2D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"Grid2D.hlsl");
		Mat->SetPixelShader(L"Grid2D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdHeightMap");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"DefferdHeightMapShader.hlsl");
		Mat->SetPixelShader(L"DefferdHeightMapShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"HeightMap");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"HeightMapShader.hlsl");
		Mat->SetPixelShader(L"HeightMapShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdStatic3DMeshTexture");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"BACK");

		Mat->SetVertexShader(L"DefferdStatic3DMeshTexture.hlsl");
		Mat->SetPixelShader(L"DefferdStatic3DMeshTexture.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DefferdStatic3DMeshColor");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"BACK");

		Mat->SetVertexShader(L"DefferdStatic3DMeshColor.hlsl");
		Mat->SetPixelShader(L"DefferdStatic3DMeshColor.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Static3DMeshColor");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"BACK");

		Mat->SetVertexShader(L"Static3DMeshColor.hlsl");
		Mat->SetPixelShader(L"Static3DMeshColor.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"DebugBox");
		Mat->SetBlend(L"NoneAlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"WIREBACK");

		Mat->SetVertexShader(L"3DMeshColor.hlsl");
		Mat->SetPixelShader(L"3DMeshColor.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"3DMeshColor");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"BACK");

		Mat->SetVertexShader(L"3DMeshColor.hlsl");
		Mat->SetPixelShader(L"3DMeshColor.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"EngineGrid3D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		
		Mat->SetVertexShader(L"EngineGrid3D.hlsl");
		Mat->SetPixelShader(L"EngineGrid3D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"FbxAniShader3D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"FbxAniShader3D.hlsl");
		Mat->SetPixelShader(L"FbxAniShader3D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"TextureShader3D");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"TextureShader3D.hlsl");
		Mat->SetPixelShader(L"TextureShader3D.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Texture2DMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"2DTextureShader.hlsl");
		Mat->SetPixelShader(L"2DTextureShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Particle");
		Mat->SetBlend(L"ParticleBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"Particle.hlsl");
		Mat->SetGeometryShader(L"Particle.hlsl");
		Mat->SetPixelShader(L"Particle.hlsl");
		Mat->SetDrawType(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Texture2DMaskMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"2DTextureMaskShader.hlsl");
		Mat->SetPixelShader(L"2DTextureMaskShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"OutLine");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"OutLine.hlsl");
		Mat->SetPixelShader(L"OutLine.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"Uvdistortion");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");

		Mat->SetVertexShader(L"Uvdistortion.hlsl");
		Mat->SetPixelShader(L"Uvdistortion.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"MaskCulling");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"MaskCulling.hlsl");
		Mat->SetPixelShader(L"MaskCulling.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"CircleMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"LessDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"CircleShader.hlsl");
		Mat->SetPixelShader(L"CircleShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"BoxMat");
		Mat->SetBlend(L"AlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"BoxShader.hlsl");
		Mat->SetPixelShader(L"BoxShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"SceneChangeStartMat");
		Mat->SetBlend(L"NoneAlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"SceneChangeStartShader.hlsl");
		Mat->SetPixelShader(L"SceneChangeStartShader.hlsl");
	}

	{
		std::shared_ptr<GJHMaterial> Mat = GJHMaterial::Create(L"SceneChangeEndMat");
		Mat->SetBlend(L"NoneAlphaBlend");
		Mat->SetDepthStencil(L"DisableDepth");
		Mat->SetRasterizer(L"NONE");
		Mat->SetVertexShader(L"SceneChangeEndShader.hlsl");
		Mat->SetPixelShader(L"SceneChangeEndShader.hlsl");
	}
}