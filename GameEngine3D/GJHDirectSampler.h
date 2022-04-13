#pragma once
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include "GJHDirectResourcesManager.h"
#include "GJHDirectShader.h"

class GJHDirectSampler : public GJHResBase, public GJHDirectResourcesManager<GJHDirectSampler>
{
public:
	static bool Create(const GJHGameEngineString& _Name, const D3D11_SAMPLER_DESC& _Desc)
	{
		GJHDirectSampler* NewRes = new GJHDirectSampler();

		if (NewRes->Create(_Desc) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	D3D11_SAMPLER_DESC m_Desc;
	ID3D11SamplerState* m_State;

public:
	void Setting(ShaderType _Type, int _Reg);
	void VSSetting(int _Reg);
	void PSSetting(int _Reg);
	void GSSetting(int _Reg);

public:
	GJHDirectSampler();
	~GJHDirectSampler();

public:
	GJHDirectSampler(const GJHDirectSampler& _Other) = delete;
	GJHDirectSampler(const GJHDirectSampler&& _Other) = delete;

public:
	GJHDirectSampler operator=(const GJHDirectSampler& _Other) = delete;
	GJHDirectSampler operator=(const GJHDirectSampler&& _Other) = delete;

public:
	bool Create(const D3D11_SAMPLER_DESC& _Desc);
};