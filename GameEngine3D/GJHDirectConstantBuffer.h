#pragma once
#include "GJHDirectBuffer.h"
#include "GJHDirectShader.h"
#include "GJHDirectResourcesManager.h"

class GJHDirectConstantBuffer :	public GJHDirectBuffer,	public GJHDirectResourcesManager<GJHDirectConstantBuffer>
{
public:
	template<typename DataType>
	static bool Create(const GJHGameEngineString& _Name, ShaderType _Type)
	{
		return Create(_Name, _Type, sizeof(DataType));
	}

	static bool Create(const GJHGameEngineString& _Name, ShaderType _Type, int _Reg, size_t _Size)
	{
		GJHDirectConstantBuffer* NewRes = new GJHDirectConstantBuffer();

		if (NewRes->Create(_Size, _Type, _Reg) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	D3D11_MAPPED_SUBRESOURCE m_Res;

public:
	GJHDirectConstantBuffer();
	~GJHDirectConstantBuffer();

public:
	GJHDirectConstantBuffer(const GJHDirectConstantBuffer& _Other) = delete;
	GJHDirectConstantBuffer(const GJHDirectConstantBuffer&& _Other) = delete;

public:
	GJHDirectConstantBuffer operator=(const GJHDirectConstantBuffer& _Other) = delete;
	GJHDirectConstantBuffer operator=(const GJHDirectConstantBuffer&& _Other) = delete;

public:
	bool Create(size_t _BufferSize, ShaderType _Type, int _Reg);
	void Setting(ShaderType _Type, int _Reg);
	void BufferInfoSetting() override;

	void VSSetting(int _Reg);
	void PSSetting(int _Reg);
	void GSSetting(int _Reg);

	template<typename DataType>
	void ChangeData(DataType& _Data)
	{
		ChangeData(&_Data, sizeof(DataType));
	}

	void ChangeDataPtr(void* _Data)
	{
		ChangeData(_Data, GetBufferInfo().ByteWidth);
	}

	void ChangeData(void* _Data, size_t _Size);
};