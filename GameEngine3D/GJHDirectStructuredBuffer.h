#pragma once
#include "GJHDirectBuffer.h"
#include "GJHDirectResourcesManager.h"
#include <vector>

class GJHDirectStructuredBuffer : public GJHDirectBuffer, public GJHDirectResourcesManager<GJHDirectStructuredBuffer>
{
public:
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11UnorderedAccessView* m_pUAV;

	unsigned int m_iDataSize;
	unsigned int m_iDataCount;

	D3D11_MAPPED_SUBRESOURCE m_Res;

public:
	void Create(UINT _iDataSize, UINT _iDataCount, void* _pSysMem);

	void CreateRW(UINT _iElementSize, UINT _iElementCount, void* _pSysMem);

	void Clear();
	void ClearRW();

	void ChangeDataPtr(void* _Data)
	{
		ChangeData(_Data, GetBufferInfo().ByteWidth);
	}

	void ChangeData(void* _Data, size_t _Size);

	void GetDataPtr(void* _Data)
	{
		GetData(_Data, GetBufferInfo().ByteWidth);
	}

	void GetData(void* _pData, size_t _Size);

public:
	GJHDirectStructuredBuffer();
	~GJHDirectStructuredBuffer();

public:
	GJHDirectStructuredBuffer(const GJHDirectStructuredBuffer& _Other) = delete;
	GJHDirectStructuredBuffer(const GJHDirectStructuredBuffer&& _Other) = delete;

public:
	GJHDirectStructuredBuffer operator=(const GJHDirectStructuredBuffer& _Other) = delete;
	GJHDirectStructuredBuffer operator=(const GJHDirectStructuredBuffer&& _Other) = delete;

public:
	void VSSetting(int _Reg);
	void PSSetting(int _Reg);
	void GSSetting(int _Reg);
	void CSSetting(int _Reg);

private:
	void Release();
};