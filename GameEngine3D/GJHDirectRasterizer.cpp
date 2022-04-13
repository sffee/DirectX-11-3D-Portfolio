#include "GJHDirectRasterizer.h"
#include <GJHGameEngineDebug.h>

GJHDirectRasterizer::GJHDirectRasterizer() :
	m_State(nullptr),
	m_Desc()
{

}

GJHDirectRasterizer::~GJHDirectRasterizer()
{
	if (m_State != nullptr)
	{
		m_State->Release();
	}
}

bool GJHDirectRasterizer::Create(const D3D11_RASTERIZER_DESC& _Desc)
{
	m_Desc = _Desc;

	if (GJHGameEngineDevice::GetDevice()->CreateRasterizerState(&m_Desc, &m_State) != S_OK)
	{
		MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateRasterizerState(&m_Desc, &m_State) != S_OK)");
		return false;
	}

	return true;
}

void GJHDirectRasterizer::Setting()
{
	if (m_State == nullptr)
	{
		MsgAssert("if (m_State == nullptr)");
	}

	GJHGameEngineDevice::GetContext()->RSSetState(m_State);
}