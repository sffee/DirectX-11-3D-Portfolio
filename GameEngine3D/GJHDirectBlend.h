#pragma once
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include "GJHDirectResourcesManager.h"

class GJHDirectBlend : public GJHResBase, public GJHDirectResourcesManager<GJHDirectBlend>
{
public:
	static bool Create(const GJHGameEngineString& _Name, const D3D11_BLEND_DESC& _Desc)
	{
		GJHDirectBlend* NewRes = new GJHDirectBlend();
		NewRes->SetName(_Name);

		if (NewRes->Create(_Desc) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	D3D11_BLEND_DESC m_Desc;
	ID3D11BlendState* m_State;
	float4 Factor;

public:
	GJHDirectBlend();
	~GJHDirectBlend();

public:
	GJHDirectBlend(const GJHDirectBlend& _Other) = delete;
	GJHDirectBlend(const GJHDirectBlend&& _Other) = delete;

public:
	GJHDirectBlend operator=(const GJHDirectBlend& _Other) = delete;
	GJHDirectBlend operator=(const GJHDirectBlend&& _Other) = delete;

public:
	bool Create(const D3D11_BLEND_DESC& _Desc);
	void Setting() override;
};