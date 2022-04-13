#include "GJHDirectStructuredBuffer.h"


void GJHDirectStructuredBuffer::Release()
{
	if (m_pSRV != nullptr)
	{
		m_pSRV->Release();
	}

	if (m_pUAV != nullptr)
	{
		m_pUAV->Release();
	}

	m_iDataSize = 0;
	m_iDataCount = 0;
}

GJHDirectStructuredBuffer::GJHDirectStructuredBuffer() :
	m_pSRV(nullptr),
	m_pUAV(nullptr)
{

}

GJHDirectStructuredBuffer::~GJHDirectStructuredBuffer()
{
	Release();
}

void GJHDirectStructuredBuffer::Create(UINT _iDataSize, UINT _iDataCount, void* _pSysMem)
{
	m_iDataSize = _iDataSize;
	m_iDataCount = _iDataCount;

	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = m_iDataSize * m_iDataCount;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tBufferDesc.StructureByteStride = m_iDataSize;

	CreateBuffer(tBufferDesc, _pSysMem);

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = m_iDataCount;

	if (GJHGameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("FAIL if (GJHGameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV) != S_OK)");
	}
}

void GJHDirectStructuredBuffer::CreateRW(UINT _iElementSize, UINT _iElementCount, void* _pSysMem)
{
	Release();

	m_iDataSize = _iElementSize;
	m_iDataCount = _iElementCount;

	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = m_iDataSize * m_iDataCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tBufferDesc.StructureByteStride = m_iDataSize;

	CreateBuffer(tBufferDesc, _pSysMem);

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = _iElementCount;
	GJHGameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};

	tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUAVDesc.Buffer.FirstElement = 0;
	tUAVDesc.Buffer.Flags = 0;
	tUAVDesc.Buffer.NumElements = _iElementCount;

	if (GJHGameEngineDevice::GetDevice()->CreateUnorderedAccessView(GetBuffer(), &tUAVDesc, &m_pUAV) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateUnorderedAccessView(GetBuffer(), &tUAVDesc, &m_pUAV) != S_OK)");
	}
}

void GJHDirectStructuredBuffer::Clear()
{

}
void GJHDirectStructuredBuffer::ClearRW()
{
	
}

void GJHDirectStructuredBuffer::GetData(void* _pData, size_t _Size)
{
#ifdef _DEBUG
	m_Res.pData = nullptr;
#endif
	GJHGameEngineDevice::GetContext()->Map(m_pBuffer, 0, D3D11_MAP_READ, 0, &m_Res);
#ifdef _DEBUG
	if (m_Res.pData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Res.pData == nullptr)");
	}
#endif 
	memcpy_s(_pData, m_BufferInfo.ByteWidth, m_Res.pData, _Size);
	
	GJHGameEngineDevice::GetContext()->Unmap(m_pBuffer, 0);
}

void GJHDirectStructuredBuffer::ChangeData(void* _pData, size_t _Size)
{
#ifdef _DEBUG
	m_Res.pData = nullptr;
#endif

	GJHGameEngineDevice::GetContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_Res);

#ifdef _DEBUG
	if (m_Res.pData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (nullptr == m_Res.pData)");
	}
#endif 

	memcpy_s(m_Res.pData, m_BufferInfo.ByteWidth, _pData, _Size);
	
	GJHGameEngineDevice::GetContext()->Unmap(m_pBuffer, 0);
}

void GJHDirectStructuredBuffer::VSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->VSSetShaderResources(_Reg, 1, &m_pSRV);
}

void GJHDirectStructuredBuffer::PSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->PSSetShaderResources(_Reg, 1, &m_pSRV);
}

void GJHDirectStructuredBuffer::GSSetting(int _Reg)
{
	GJHGameEngineDevice::GetContext()->GSSetShaderResources(_Reg, 1, &m_pSRV);
}

void GJHDirectStructuredBuffer::CSSetting(int _Reg)
{
	unsigned int Index = -1;
	GJHGameEngineDevice::GetContext()->CSSetUnorderedAccessViews(_Reg, 1, &m_pUAV, &Index);
}