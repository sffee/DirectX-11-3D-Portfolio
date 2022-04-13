#pragma once
#include "GJHDirectBuffer.h"
#include "GJHDirectResourcesManager.h"
#include <vector>

class GJHDirectIndexBuffer : public GJHDirectBuffer, public GJHDirectResourcesManager<GJHDirectIndexBuffer>
{
public:
	template<typename IdxType>
	static bool Create(const GJHGameEngineString& _Name, std::vector<IdxType>& _Type, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		GJHDirectIndexBuffer* NewRes = new GJHDirectIndexBuffer();

		DXGI_FORMAT _Fmt;

		if (sizeof(IdxType) == 2)
		{
			_Fmt = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		}
		else if (sizeof(IdxType) == 4)
		{
			_Fmt = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}

		if (NewRes->Create(&_Type[0], sizeof(IdxType), _Type.size(), _Fmt, _Usage) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	DXGI_FORMAT	m_Fmt;
	unsigned int m_IndexSize;
	unsigned int m_IndexCount;
	unsigned int m_Offset;

public:
	unsigned int GetIndexCount()
	{
		return m_IndexCount;
	}

public:
	GJHDirectIndexBuffer();
	~GJHDirectIndexBuffer();

public:
	GJHDirectIndexBuffer(const GJHDirectIndexBuffer& _Other) = delete;
	GJHDirectIndexBuffer(const GJHDirectIndexBuffer&& _Other) = delete;

public:
	GJHDirectIndexBuffer operator=(const GJHDirectIndexBuffer& _Other) = delete;
	GJHDirectIndexBuffer operator=(const GJHDirectIndexBuffer&& _Other) = delete;

public:
	void BufferInfoSetting() override;
	bool Create(void* _StartData, size_t _IndexSize, size_t _IndexCount, DXGI_FORMAT _Fmt, D3D11_USAGE _Usage);
	void Setting() override;
};