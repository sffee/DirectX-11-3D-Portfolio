#include "GJHDirectConstantBuffer.h"
#include "GJHGameEngineDevice.h"

GJHDirectConstantBuffer::GJHDirectConstantBuffer() :
	m_Res()
{

}

GJHDirectConstantBuffer::~GJHDirectConstantBuffer()
{

}

void GJHDirectConstantBuffer::BufferInfoSetting()
{

}

bool GJHDirectConstantBuffer::Create(size_t _BufferSize, ShaderType _Type, int _Reg)
{
	m_BufferInfo.ByteWidth = (UINT)_BufferSize;

	m_BufferInfo.CPUAccessFlags = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	m_BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (CreateConstBuffer(nullptr, _Type, _Reg, D3D11_USAGE::D3D11_USAGE_DYNAMIC) == false)
	{
		return false;
	}

	return true;
}

void GJHDirectConstantBuffer::Setting(ShaderType _Type, int _Reg)
{
	switch (_Type)
	{
	case ShaderType::Vertex:
		VSSetting(_Reg);
		break;
	case ShaderType::Hull:
	case ShaderType::Geometry:
	case ShaderType::Domain:
		GJHGameEngineDebug::AssertMsg("case ShaderType error");
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

void GJHDirectConstantBuffer::VSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->VSSetConstantBuffers(_Reg, 1, &m_pBuffer);
}

void GJHDirectConstantBuffer::GSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->GSSetConstantBuffers(_Reg, 1, &m_pBuffer);
}

void GJHDirectConstantBuffer::PSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->PSSetConstantBuffers(_Reg, 1, &m_pBuffer);
}

void GJHDirectConstantBuffer::ChangeData(void* _Data, size_t _Size)
{
#ifdef _DEBUG
	if (m_BufferInfo.ByteWidth != _Size)
	{
		GJHGameEngineDebug::AssertMsg("if (m_BufferInfo.ByteWidth != _Size)");
	}
	
	m_Res.pData = nullptr;
#endif

	GJHGameEngineDevice::GetContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_Res);

#ifdef _DEBUG
	if (m_Res.pData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Res.pData == nullptr)");
	}
#endif

	memcpy_s(m_Res.pData, m_BufferInfo.ByteWidth, _Data, _Size);

	GJHGameEngineDevice::GetContext()->Unmap(m_pBuffer, 0);
}