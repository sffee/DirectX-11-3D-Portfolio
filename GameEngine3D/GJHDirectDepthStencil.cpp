#include "GJHDirectDepthStencil.h"
#include <GJHGameEngineDebug.h>

GJHDirectDepthStencil::GJHDirectDepthStencil() :
	m_State(nullptr),
	m_Desc()
{

}

GJHDirectDepthStencil::~GJHDirectDepthStencil()
{
	if (m_State != nullptr)
	{
		m_State->Release();
	}
}

bool GJHDirectDepthStencil::Create(const D3D11_DEPTH_STENCIL_DESC& _Desc)
{
	m_Desc = _Desc;

	if (GJHGameEngineDevice::GetDevice()->CreateDepthStencilState(&m_Desc, &m_State) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateDepthStencilState(&m_Desc, &m_State) != S_OK)");
		return false;
	}

	return true;
}

void GJHDirectDepthStencil::Setting()
{
	if (m_State == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_State == nullptr)");
	}

	GJHGameEngineDevice::GetContext()->OMSetDepthStencilState(m_State, 0);
}