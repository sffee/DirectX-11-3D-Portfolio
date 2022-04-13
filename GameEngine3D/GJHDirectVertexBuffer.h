#pragma once
#include "GJHDirectBuffer.h"
#include "GJHDirectResourcesManager.h"
#include <vector>

class GJHDirectVertexBuffer : public GJHDirectBuffer, public GJHDirectResourcesManager<GJHDirectVertexBuffer>
{
public:
	template<typename VtxType>
	static bool Create(const GJHGameEngineString& _Name, std::vector<VtxType>& _Type, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		GJHDirectVertexBuffer* NewRes = new GJHDirectVertexBuffer();
		if (NewRes->Create(&_Type[0], sizeof(VtxType), _Type.size(), _Usage) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	unsigned int m_VertexSize;
	unsigned int m_VertexCount;
	std::vector<unsigned int> m_Offset;

public:
	GJHDirectVertexBuffer();
	~GJHDirectVertexBuffer();
	GJHDirectVertexBuffer(const GJHDirectVertexBuffer& _Other) = delete;
	GJHDirectVertexBuffer(const GJHDirectVertexBuffer&& _Other) = delete;

public:
	GJHDirectVertexBuffer operator=(const GJHDirectVertexBuffer& _Other) = delete;
	GJHDirectVertexBuffer operator=(const GJHDirectVertexBuffer&& _Other) = delete;

public:
	void BufferInfoSetting() override;
	bool Create(void* _StartData, size_t _VertexSize, size_t _VertexCount, D3D11_USAGE _Usage);
	void Setting() override;
};