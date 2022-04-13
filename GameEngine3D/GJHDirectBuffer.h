#pragma once
#include "GJHGameEngineDevice.h"
#include <BaseMinimalHeader.h>
#include "GJHResBase.h"
#include <GJHGameEngineDebug.h>
#include <vector>
#include <map>
#include "GJHDirectShader.h"

class GJHGameEngineDevice;
class GJHDirectBuffer : public GJHResBase
{
private:
	friend GJHGameEngineDevice;

private:
	static std::map<int, std::map<ShaderType, std::map<int, ID3D11Buffer*>>> ConstBufferPool;

private:
	static void ConstBufferPoolRelease()
	{
		for (auto& Buffers : ConstBufferPool)
		{
			for (auto& Buffer : Buffers.second)
			{
				for (auto& Buf : Buffer.second)
				{
					Buf.second->Release();
				}
			}
		}
	}

protected:
	ID3D11Buffer* m_pBuffer;
	D3D11_SUBRESOURCE_DATA m_StartData;

protected:
	D3D11_BUFFER_DESC m_BufferInfo;

public:
	ID3D11Buffer* GetBuffer()
	{
		return m_pBuffer;
	}

	const D3D11_BUFFER_DESC& GetBufferInfo()
	{
		return m_BufferInfo;
	}

public:
	void StartDataSetting(void* _StartData)
	{
		m_StartData.pSysMem = _StartData;
	}

	void UsageSetting(D3D11_USAGE _Usage)
	{
		m_BufferInfo.Usage = _Usage;

		if (m_BufferInfo.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
		{
			m_BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	}

public:
	GJHDirectBuffer() :
		m_pBuffer(nullptr),
		m_BufferInfo{},
		m_StartData{ 0, }
	{

	};

	virtual ~GJHDirectBuffer()
	{
		if (m_BufferInfo.BindFlags == D3D11_BIND_CONSTANT_BUFFER)
		{
			return;
		}

		if (m_pBuffer != nullptr)
		{
			m_pBuffer->Release();
		}
	}

public:
	GJHDirectBuffer(const GJHDirectBuffer& _Other) = delete;
	GJHDirectBuffer(const GJHDirectBuffer&& _Other) = delete;

public:
	GJHDirectBuffer operator=(const GJHDirectBuffer& _Other) = delete;
	GJHDirectBuffer operator=(const GJHDirectBuffer&& _Other) = delete;

public:
	virtual void BufferInfoSetting() { }

	bool CreateBuffer(const D3D11_BUFFER_DESC& _BufferInfo, void* _StartData)
	{
		StartDataSetting(_StartData);
		m_BufferInfo = _BufferInfo;

		if (_StartData == nullptr)
		{
			if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffer) != S_OK)
			{
				MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffers[m_pBuffers.size() - 1]) != S_OK)");
				return false;
			}
		}
		else
		{
			if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, &m_StartData, &m_pBuffer) != S_OK)
			{
				MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffers[m_pBuffers.size() - 1]) != S_OK)");
				return false;
			}
		}

		return true;
	}

	bool CreateBuffer(void* _StartData, D3D11_USAGE _Usage)
	{
		StartDataSetting(_StartData);
		UsageSetting(_Usage);
		BufferInfoSetting();

		if (_StartData == nullptr)
		{
			if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffer) != S_OK)
			{
				MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffers[m_pBuffers.size() - 1]) != S_OK)");
				return false;
			}
		}
		else
		{
			if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, &m_StartData, &m_pBuffer) != S_OK)
			{
				MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffers[m_pBuffers.size() - 1]) != S_OK)");
				return false;
			}
		}

		return true;
	}

	bool CreateConstBuffer(void* _StartData, ShaderType _Type, int _Reg, D3D11_USAGE _Usage)
	{
		StartDataSetting(_StartData);
		UsageSetting(_Usage);
		BufferInfoSetting();

		if (ConstBufferPool.find(m_BufferInfo.ByteWidth) != ConstBufferPool.end())
		{
			if (ConstBufferPool[m_BufferInfo.ByteWidth].find(_Type) != ConstBufferPool[m_BufferInfo.ByteWidth].end())
			{
				if (ConstBufferPool[m_BufferInfo.ByteWidth][_Type].find(_Reg) != ConstBufferPool[m_BufferInfo.ByteWidth][_Type].end())
				{
					m_pBuffer = ConstBufferPool[m_BufferInfo.ByteWidth][_Type][_Reg];
					return true;
				}
			}
		}

		if (_StartData == nullptr)
		{
			if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, nullptr, &m_pBuffer) != S_OK)
			{
				GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo. nullptr, &m_pBuffer) != S_OK)");
				return false;
			}
		}
		else
		{
			if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, &m_StartData, &m_pBuffer) != S_OK)
			{
				GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateBuffer(&m_BufferInfo, &m_StartData, &m_pBuffer) != S_OK)");
				return false;
			}
		}

		ConstBufferPool[m_BufferInfo.ByteWidth][_Type][_Reg] = m_pBuffer;

		return true;
	}
};