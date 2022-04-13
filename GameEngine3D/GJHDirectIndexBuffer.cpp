#include "GJHDirectIndexBuffer.h"
#include "GJHGameEngineDevice.h"

GJHDirectIndexBuffer::GJHDirectIndexBuffer() :
	m_Fmt(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN),
	m_IndexSize(0),
	m_IndexCount(0),
	m_Offset(0)
{

}

GJHDirectIndexBuffer::~GJHDirectIndexBuffer()
{

}

void GJHDirectIndexBuffer::BufferInfoSetting()
{
	m_BufferInfo.ByteWidth = m_IndexSize * m_IndexCount;
	m_BufferInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
}

bool GJHDirectIndexBuffer::Create(void* _StartData, size_t _IndexSize, size_t _IndexCount, DXGI_FORMAT _Fmt, D3D11_USAGE _Usage)
{
	m_Fmt = _Fmt;
	m_IndexSize = (unsigned int)_IndexSize;
	m_IndexCount = (unsigned int)_IndexCount;

	if (CreateBuffer(_StartData, _Usage) == false)
	{
		return false;
	}

	return true;
}

void GJHDirectIndexBuffer::Setting()
{
	GJHGameEngineDevice::GetContext()->IASetIndexBuffer(m_pBuffer, m_Fmt, m_Offset);
}