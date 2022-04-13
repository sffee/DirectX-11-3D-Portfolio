#include "GJHDirectSampler.h"
#include <GJHGameEngineDebug.h>

GJHDirectSampler::GJHDirectSampler() :
	m_State(nullptr),
	m_Desc()
{

}

GJHDirectSampler::~GJHDirectSampler()
{
	if (m_State != nullptr)
	{
		m_State->Release();
		m_State = nullptr;
	}
}

bool GJHDirectSampler::Create(const D3D11_SAMPLER_DESC& _Desc)
{
	m_Desc = _Desc;

	if (GJHGameEngineDevice::GetDevice()->CreateSamplerState(&m_Desc, &m_State) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateSamplerState(&m_Desc, &m_State) != S_OK)");
		return false;
	}

	return true;
}

void GJHDirectSampler::Setting(ShaderType _Type, int _Reg)
{
	switch (_Type)
	{
	case ShaderType::Vertex:
		VSSetting(_Reg);
		break;
	case ShaderType::Hull:
	case ShaderType::Geometry:
	case ShaderType::Domain:
		GJHGameEngineDebug::AssertMsg("case ShaderType Error");
		break;
	case ShaderType::Pixel:
		PSSetting(_Reg);
		break;
	case ShaderType::End:
		break;
	default:
		break;
	}
}

void GJHDirectSampler::VSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->VSSetSamplers(_Reg, 1, &m_State);
}

void GJHDirectSampler::PSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->PSSetSamplers(_Reg, 1, &m_State);
}

void GJHDirectSampler::GSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->GSSetSamplers(_Reg, 1, &m_State);
}