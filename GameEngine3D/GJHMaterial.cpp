#include "GJHMaterial.h"
#include "GJHDirectRasterizer.h"
#include "GJHDirectPixelShader.h"
#include "GJHDirectGeometryShader.h"
#include "GJHDirectVertexShader.h"
#include <GJHGameEngineDebug.h>
#include "GJHDirectDepthStencil.h"
#include "GJHDirectBlend.h"

GJHDirectVertexShader* GJHMaterial::gLastVertexShader = nullptr;
GJHDirectGeometryShader* GJHMaterial::gLastGeometryShader = nullptr;
GJHDirectRasterizer* GJHMaterial::gLastRasterizer = nullptr;
GJHDirectPixelShader* GJHMaterial::gLastPixelShader = nullptr;
GJHDirectBlend* GJHMaterial::gLastBlend = nullptr;
GJHDirectDepthStencil* GJHMaterial::gLastDepthStencil = nullptr;

void GJHMaterial::GlobalResourcesReset()
{
	gLastVertexShader = nullptr;
	gLastGeometryShader = nullptr;
	gLastRasterizer = nullptr;
	gLastPixelShader = nullptr;
	gLastBlend = nullptr;
	gLastDepthStencil = nullptr;
}

GJHMaterial::GJHMaterial() :
	m_IsOriginal(true),
	m_DrawType(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{

}

GJHMaterial::~GJHMaterial()
{

}

std::shared_ptr<GJHMaterial> GJHMaterial::Clone()
{
	std::shared_ptr<GJHMaterial> CloneMat = std::make_shared<GJHMaterial>();

	CloneMat->m_IsOriginal = false;
	CloneMat->VertexShader = VertexShader;
	CloneMat->Rasterizer = Rasterizer;
	CloneMat->PixelShader = PixelShader;
	CloneMat->Blend = Blend;
	CloneMat->DepthStencil = DepthStencil;

	return CloneMat;
}

void GJHMaterial::SetVertexShader(const GJHGameEngineString& _Name)
{
	VertexShader = GJHDirectVertexShader::Find(_Name);

	if (VertexShader == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (VertexShader == nullptr)");
	}
}

void GJHMaterial::SetGeometryShader(const GJHGameEngineString& _Name)
{
	GeometryShader = GJHDirectGeometryShader::Find(_Name);

	if (GeometryShader == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (GeometryShader == nullptr)");
	}
}

void GJHMaterial::SetPixelShader(const GJHGameEngineString& _Name)
{
	PixelShader = GJHDirectPixelShader::Find(_Name);

	if (PixelShader == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (PixelShader == nullptr)");
	}
}

void GJHMaterial::SetRasterizer(const GJHGameEngineString& _Name)
{
	Rasterizer = GJHDirectRasterizer::Find(_Name);

	if (Rasterizer == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Rasterizer == nullptr)");
	}
}

void GJHMaterial::SetDepthStencil(const GJHGameEngineString& _Name)
{
	DepthStencil = GJHDirectDepthStencil::Find(_Name);

	if (DepthStencil == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (DepthStencil == nullptr)");
	}
}

void GJHMaterial::SetBlend(const GJHGameEngineString& _Name)
{
	Blend = GJHDirectBlend::Find(_Name);

	if (Blend == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (Blend == nullptr)");
	}
}

void GJHMaterial::Setting()
{
	GJHGameEngineDevice::GetContext()->IASetPrimitiveTopology(m_DrawType);

	if (VertexShader != nullptr && gLastVertexShader != VertexShader.get())
	{
		gLastVertexShader = VertexShader.get();
		VertexShader->Setting();
	}
	else if (VertexShader == nullptr && gLastVertexShader != nullptr)
	{
		gLastVertexShader = nullptr;
		GJHGameEngineDevice::VtxShaderReset();
	}

	if (GeometryShader != nullptr && gLastGeometryShader != GeometryShader.get())
	{
		gLastGeometryShader = GeometryShader.get();
		GeometryShader->Setting();
	}
	else if (GeometryShader == nullptr && gLastGeometryShader != nullptr)
	{
		gLastGeometryShader = nullptr;
		GJHGameEngineDevice::GeoShaderReset();
	}

	if (PixelShader != nullptr && gLastPixelShader != PixelShader.get())
	{
		gLastPixelShader = PixelShader.get();
		PixelShader->Setting();
	}
	else if (PixelShader == nullptr && gLastPixelShader != nullptr)
	{
		gLastPixelShader = nullptr;
		GJHGameEngineDevice::PixShaderReset();
	}

	if (Rasterizer != nullptr && gLastRasterizer != Rasterizer.get())
	{
		gLastRasterizer = Rasterizer.get();
		Rasterizer->Setting();
	}
	else if (Rasterizer == nullptr && gLastRasterizer != nullptr)
	{
		gLastRasterizer = nullptr;
		GJHGameEngineDevice::RasterizerReset();
	}

	if (DepthStencil != nullptr && gLastDepthStencil != DepthStencil.get())
	{
		gLastDepthStencil = DepthStencil.get();
		DepthStencil->Setting();
	}
	else if (DepthStencil == nullptr && gLastDepthStencil != nullptr)
	{
		gLastDepthStencil = nullptr;
		GJHGameEngineDevice::DepthStencilReset();
	}

	if (Blend != nullptr && gLastBlend != Blend.get())
	{
		gLastBlend = Blend.get();
		Blend->Setting();
	}
	else if (Blend == nullptr && gLastBlend != nullptr)
	{
		gLastBlend = nullptr;
		GJHGameEngineDevice::BlendReset();
	}
}