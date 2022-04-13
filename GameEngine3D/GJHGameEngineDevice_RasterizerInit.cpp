#include "GJHGameEngineDevice.h"
#include "GJHDirectRasterizer.h"

void GJHGameEngineDevice::RasterizerInit()
{
	{
		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;
		GJHDirectRasterizer::Create(L"BACK", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK, 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;
		GJHDirectRasterizer::Create(L"WIREBACK", Desc);
	}
	
	{
		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE, 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;
		GJHDirectRasterizer::Create(L"NONE", Desc);
	}
}