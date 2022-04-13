#include "GJHGameEngineDevice.h"
#include "GJHDirectSampler.h"

void GJHGameEngineDevice::SamplerInit()
{
	{
		D3D11_SAMPLER_DESC SmpInfo;

		SmpInfo.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SmpInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SmpInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SmpInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		SmpInfo.BorderColor[0] = 1.f;
		SmpInfo.BorderColor[1] = 0.f;
		SmpInfo.BorderColor[2] = 0.f;
		SmpInfo.BorderColor[3] = 1.f;

		SmpInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;

		SmpInfo.MaxAnisotropy = 0;

		SmpInfo.MinLOD = -FLT_MAX;
		SmpInfo.MaxLOD = FLT_MAX;
		SmpInfo.MipLODBias = 1.f;

		GJHDirectSampler::Create(L"SMPLINEAR", SmpInfo);
	}

	{
		D3D11_SAMPLER_DESC SmpInfo;

		SmpInfo.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		SmpInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		SmpInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		SmpInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

		SmpInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;

		SmpInfo.MaxAnisotropy = 0;

		SmpInfo.MinLOD = -FLT_MAX;
		SmpInfo.MaxLOD = FLT_MAX;
		SmpInfo.MipLODBias = 1.f;

		GJHDirectSampler::Create(L"SMPPOINT", SmpInfo);
	}

	{
		D3D11_SAMPLER_DESC SmpInfo;

		SmpInfo.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SmpInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		SmpInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		SmpInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

		SmpInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;
		SmpInfo.MaxAnisotropy = 0;
		SmpInfo.MinLOD = -FLT_MAX;
		SmpInfo.MinLOD = FLT_MAX;
		SmpInfo.MipLODBias = 1.0F;

		GJHDirectSampler::Create(L"SHADOWSMP", SmpInfo);
	}
}