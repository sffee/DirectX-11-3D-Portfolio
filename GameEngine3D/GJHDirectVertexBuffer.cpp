#include "GJHDirectVertexBuffer.h"
#include "GJHGameEngineDevice.h"

GJHDirectVertexBuffer::GJHDirectVertexBuffer() :
	m_VertexSize(0),
	m_VertexCount(0),
	m_Offset(0)
{

}

GJHDirectVertexBuffer::~GJHDirectVertexBuffer()
{

}

void GJHDirectVertexBuffer::BufferInfoSetting()
{
	m_BufferInfo.ByteWidth = m_VertexSize * m_VertexCount;
	m_BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
}

bool GJHDirectVertexBuffer::Create(void* _StartData, size_t _VertexSize, size_t _VertexCount, D3D11_USAGE _Usage)
{
	m_VertexSize = (unsigned int)_VertexSize;
	m_VertexCount = (unsigned int)_VertexCount;

	if (CreateBuffer(_StartData, _Usage) == false)
	{
		return false;
	}

	m_Offset.push_back(0);

	return true;
}

void GJHDirectVertexBuffer::Setting()
{
	GJHGameEngineDevice::GetContext()->IASetVertexBuffers(0, 1, &m_pBuffer, &m_VertexSize, &m_Offset[0]);
}