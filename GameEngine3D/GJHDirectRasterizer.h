#pragma once
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include "GJHDirectResourcesManager.h"

class GJHDirectRasterizer : public GJHResBase, public GJHDirectResourcesManager<GJHDirectRasterizer>
{
public:
	static bool Create(const GJHGameEngineString& _Name, const D3D11_RASTERIZER_DESC& _Desc)
	{
		GJHDirectRasterizer* NewRes = new GJHDirectRasterizer();

		if (NewRes->Create(_Desc) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	D3D11_RASTERIZER_DESC m_Desc;
	ID3D11RasterizerState* m_State;

public:
	GJHDirectRasterizer();
	~GJHDirectRasterizer();

public:
	GJHDirectRasterizer(const GJHDirectRasterizer& _Other) = delete;
	GJHDirectRasterizer(const GJHDirectRasterizer&& _Other) = delete;

public:
	GJHDirectRasterizer operator=(const GJHDirectRasterizer& _Other) = delete;
	GJHDirectRasterizer operator=(const GJHDirectRasterizer&& _Other) = delete;

public:
	bool Create(const D3D11_RASTERIZER_DESC& _Desc);
	void Setting() override;
};