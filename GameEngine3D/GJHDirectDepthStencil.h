#pragma once
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include "GJHDirectResourcesManager.h"

class GJHDirectDepthStencil : public GJHResBase, public GJHDirectResourcesManager<GJHDirectDepthStencil>
{
public:
	static bool Create(const GJHGameEngineString& _Name, const D3D11_DEPTH_STENCIL_DESC& _Desc)
	{
		GJHDirectDepthStencil* NewRes = new GJHDirectDepthStencil();

		if (NewRes->Create(_Desc) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	D3D11_DEPTH_STENCIL_DESC m_Desc;
	ID3D11DepthStencilState* m_State;

public:
	GJHDirectDepthStencil();
	~GJHDirectDepthStencil();

public:
	GJHDirectDepthStencil(const GJHDirectDepthStencil& _Other) = delete;
	GJHDirectDepthStencil(const GJHDirectDepthStencil&& _Other) = delete;

public:
	GJHDirectDepthStencil operator=(const GJHDirectDepthStencil& _Other) = delete;
	GJHDirectDepthStencil operator=(const GJHDirectDepthStencil&& _Other) = delete;

public:
	bool Create(const D3D11_DEPTH_STENCIL_DESC& _Desc);
	void Setting() override;
};