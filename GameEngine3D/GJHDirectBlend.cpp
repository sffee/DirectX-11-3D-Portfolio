#include "GJHDirectBlend.h"
#include <GJHGameEngineDebug.h>

GJHDirectBlend::GJHDirectBlend() :
	m_State(nullptr),
	m_Desc()
{

}

GJHDirectBlend::~GJHDirectBlend()
{
	if (m_State != nullptr)
	{
		m_State->Release();
	}
}

bool GJHDirectBlend::Create(const D3D11_BLEND_DESC& _Desc)
{
	m_Desc = _Desc;

	if (GJHGameEngineDevice::GetDevice()->CreateBlendState(&m_Desc, &m_State) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateBlendState(&m_Desc, &m_State) != S_OK)");
		return false;
	}

	return true;
}

void GJHDirectBlend::Setting()
{
	if (m_State == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_State == nullptr)");
	}

	GJHGameEngineDevice::GetContext()->OMSetBlendState(m_State, Factor.Arr, 0xffffffff);
}