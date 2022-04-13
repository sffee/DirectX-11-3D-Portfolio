#pragma once
#include <GJHGameEngineMemory.h>
#include <GJHGameEngineString.h>
#include "GJHGameEngineDevice.h"
#include "GJHDirectResourcesManager.h"

class GJHDirectVertexShader;
class GJHDirectGeometryShader;
class GJHDirectPixelShader;
class GJHDirectRasterizer;
class GJHDirectDepthStencil;
class GJHDirectBlend;
class GJHMaterial : public GJHResBase, public GJHDirectResourcesManager<GJHMaterial>
{
private:
	static GJHDirectVertexShader* gLastVertexShader;
	static GJHDirectGeometryShader* gLastGeometryShader;
	static GJHDirectRasterizer* gLastRasterizer;
	static GJHDirectPixelShader* gLastPixelShader;
	static GJHDirectBlend* gLastBlend;
	static GJHDirectDepthStencil* gLastDepthStencil;

public:
	friend class GJHRenderPlayer;

public:
	static void GlobalResourcesReset();
	static std::shared_ptr<GJHMaterial> Create(const GJHGameEngineString& _Name)
	{
		GJHMaterial* NewRes = new GJHMaterial();
		std::shared_ptr<GJHMaterial> NewPtr = InsertRes(_Name, NewRes);

		return NewPtr;
	}

private:
	bool m_IsOriginal;
	D3D11_PRIMITIVE_TOPOLOGY m_DrawType;

	std::shared_ptr<GJHDirectVertexShader> VertexShader;
	std::shared_ptr<GJHDirectGeometryShader> GeometryShader;
	std::shared_ptr<GJHDirectRasterizer> Rasterizer;
	std::shared_ptr<GJHDirectPixelShader> PixelShader;
	std::shared_ptr<GJHDirectBlend> Blend;
	std::shared_ptr<GJHDirectDepthStencil> DepthStencil;

public:
	void SetDrawType(D3D11_PRIMITIVE_TOPOLOGY _DrawType)
	{
		m_DrawType = _DrawType;
	}

	void SetVertexShader(const GJHGameEngineString& _Name);
	void SetGeometryShader(const GJHGameEngineString& _Name);
	void SetPixelShader(const GJHGameEngineString& _Name);
	void SetRasterizer(const GJHGameEngineString& _Name);
	void SetDepthStencil(const GJHGameEngineString& _Name);
	void SetBlend(const GJHGameEngineString& _Name);

public:
	std::shared_ptr<GJHMaterial> Clone();

public:
	std::shared_ptr<GJHDirectVertexShader> GetVertexShader()
	{
		return VertexShader;
	}

	std::shared_ptr<GJHDirectGeometryShader> GetGeometryShader()
	{
		return GeometryShader;
	}

	std::shared_ptr<GJHDirectPixelShader> GetPixelShader()
	{
		return PixelShader;
	}

	std::shared_ptr<GJHDirectRasterizer> GetRasterizer()
	{
		return Rasterizer;
	}

	std::shared_ptr<GJHDirectDepthStencil> GetDepthStencil()
	{
		return DepthStencil;
	}

	std::shared_ptr<GJHDirectBlend> GetBlend()
	{
		return Blend;
	}

public:
	GJHMaterial();
	~GJHMaterial();

public:
	GJHMaterial(const GJHMaterial& _Other) = delete;
	GJHMaterial(const GJHMaterial&& _Other) = delete;

public:
	GJHMaterial operator=(const GJHMaterial& _Other) = delete;
	GJHMaterial operator=(const GJHMaterial&& _Other) = delete;

public:
	void Setting() override;
};