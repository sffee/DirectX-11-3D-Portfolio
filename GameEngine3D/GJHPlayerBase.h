#pragma once
#include <vector>
#include <GJHUpdaterBase.h>
#include "GJHDirectShader.h"
#include "GJHDirectTexture2D.h"
#include "GJHDirectSampler.h"
#include "GJHDirectConstantBuffer.h"
#include "GJHDirectStructuredBuffer.h"

class ShaderSetter
{
public:
	GJHGameEngineString Name;
	ShaderType m_Type;
	int m_Reg;
};

class TextureSetter : public ShaderSetter
{
public:
	std::shared_ptr<GJHDirectTexture2D> m_Texture;
	GJHDirectTexture2D* m_Ptr;
	void(GJHDirectTexture2D::* SettingFunc)(int _Reg);
	void(GJHDirectTexture2D::* ResetFunc)(int _Reg);

public:
	void Init(std::shared_ptr<GJHDirectTexture2D> _Res)
	{
		m_Texture = _Res;
		m_Ptr = _Res.get();

		switch (m_Type)
		{
		case ShaderType::Vertex:
			SettingFunc = &GJHDirectTexture2D::VSSetting;
			ResetFunc = &GJHDirectTexture2D::VSReset;
			break;
		case ShaderType::Hull:
			break;
		case ShaderType::Geometry:
			SettingFunc = &GJHDirectTexture2D::GSSetting;
			ResetFunc = &GJHDirectTexture2D::GSReset;
			break;
		case ShaderType::Domain:
			break;
		case ShaderType::Pixel:
			SettingFunc = &GJHDirectTexture2D::PSSetting;
			ResetFunc = &GJHDirectTexture2D::PSReset;
			break;
		case ShaderType::End:
			break;
		default:
			break;
		}
	}

public:
	void Setting()
	{
		(m_Ptr->*SettingFunc)(m_Reg);
	}

	void Reset()
	{
		(m_Ptr->*ResetFunc)(m_Reg);
	}
};

class SamplerSetter : public ShaderSetter
{
public:
	std::shared_ptr<GJHDirectSampler> m_Sampler;
	GJHDirectSampler* m_Ptr;
	void(GJHDirectSampler::* SettingFunc)(int _Reg);

public:
	void Init(std::shared_ptr<GJHDirectSampler> _Res)
	{
		m_Sampler = _Res;
		m_Ptr = _Res.get();

		switch (m_Type)
		{
		case ShaderType::Vertex:
			SettingFunc = &GJHDirectSampler::VSSetting;
			break;
		case ShaderType::Hull:
			break;
		case ShaderType::Geometry:
			SettingFunc = &GJHDirectSampler::GSSetting;
			break;
		case ShaderType::Domain:
			break;
		case ShaderType::Pixel:
			SettingFunc = &GJHDirectSampler::PSSetting;
			break;
		case ShaderType::End:
			break;
		default:
			break;
		}
	}

public:
	void Setting()
	{
		(m_Ptr->*SettingFunc)(m_Reg);
	}

};

enum class BUFFERMODE
{
	LINK,
	NEW,
	END,
};


class SBufferSetter : public ShaderSetter
{
public:
	std::shared_ptr<GJHDirectStructuredBuffer> SBuffer;
	GJHDirectStructuredBuffer* m_Ptr;
	void(GJHDirectStructuredBuffer::* SettingFunc)(int _Reg);
	BUFFERMODE m_Mode;
	void* m_Data;

	SBufferSetter() :
		m_Data(nullptr),
		m_Mode(BUFFERMODE::END)
	{

	}

	~SBufferSetter()
	{
		DeleteData();
	}

	void Init()
	{
		m_Ptr = SBuffer.get();

		switch (m_Type)
		{
		case ShaderType::Vertex:
			SettingFunc = &GJHDirectStructuredBuffer::VSSetting;
			break;
		case ShaderType::Hull:
			break;
		case ShaderType::Geometry:
			SettingFunc = &GJHDirectStructuredBuffer::GSSetting;
			break;
		case ShaderType::Domain:
			break;
		case ShaderType::Pixel:
			SettingFunc = &GJHDirectStructuredBuffer::PSSetting;
			break;
		case ShaderType::End:
			break;
		default:
			break;
		}
	}

	void DeleteData()
	{
		if (m_Mode == BUFFERMODE::NEW)
		{
			if (m_Data != nullptr)
			{
				delete[] reinterpret_cast<char*>(m_Data);
			}
		}
	}

	void SettingComputeBuffer(std::shared_ptr<GJHDirectStructuredBuffer> _OtherBuffer)
	{
		DeleteData();

		if (_OtherBuffer == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (_OtherBuffer == nullptr)");
		}

		m_Mode = BUFFERMODE::LINK;
		SBuffer = _OtherBuffer;
		m_Ptr = SBuffer.get();
	}

	void SettingMode(BUFFERMODE _Mode, void* _Data, UINT _iDataSize, UINT _iDataCount)
	{
		if (SBuffer->GetBuffer() == nullptr)
		{
			SBuffer->Create(_iDataSize, _iDataCount, _Data);
		}

		if (m_Mode != _Mode)
		{
			DeleteData();
		}
		
		m_Mode = _Mode;

		switch (_Mode)
		{
		case BUFFERMODE::LINK:
			m_Data = _Data;
			break;
		case BUFFERMODE::NEW:
			m_Data = new char[SBuffer->GetBufferInfo().ByteWidth];
			if (_Data != nullptr)
			{
				memcpy_s(m_Data, _iDataSize * _iDataCount, _Data, _iDataSize * _iDataCount);
			}
			break;
		default:
			break;
		}
	}

	void Setting()
	{
		if (m_Mode == BUFFERMODE::END)
		{
			GJHGameEngineDebug::AssertMsg("if (m_Mode ==CBUFFERMODE::END) 스트럭쳐드버퍼를 세팅안했습니다.");
		}

		if (nullptr == SBuffer->GetBuffer())
		{
			GJHGameEngineDebug::AssertMsg("if (m_Mode ==CBUFFERMODE::END) 스트럭쳐드버퍼를 만들지 않았습니다..");
		}

		if (m_Data != nullptr)
		{
			SBuffer->ChangeDataPtr(m_Data);
		}

		(m_Ptr->*SettingFunc)(m_Reg);
	}

};

enum class RWBUFFERMODE
{
	GPU,
	CPU,
	END,
};

class RBufferSetter : public ShaderSetter
{
public:
	std::shared_ptr<GJHDirectStructuredBuffer> RBuffer;
	GJHDirectStructuredBuffer* m_Ptr;
	void(GJHDirectStructuredBuffer::* SettingFunc)(int _Reg);
	BUFFERMODE m_Mode;
	RWBUFFERMODE m_RWMode;
	void* m_Data; 

	RBufferSetter() : m_Data(nullptr), m_Mode(BUFFERMODE::END), m_RWMode(RWBUFFERMODE::END)
	{

	}

	~RBufferSetter()
	{
		DeleteData();
	}

	void Init()
	{
		m_Ptr = RBuffer.get();

		SettingFunc = &GJHDirectStructuredBuffer::CSSetting;
	}

	void DeleteData()
	{
		if (BUFFERMODE::NEW == m_Mode)
		{
			if (m_Data != nullptr)
			{
				delete[] reinterpret_cast<char*>(m_Data);
			}
		}
	}

	void SettingMode(BUFFERMODE _Mode, RWBUFFERMODE _RWMode, void* _Data, UINT _iDataSize, UINT _iDataCount)
	{
		if (nullptr == RBuffer->GetBuffer())
		{
			RBuffer->CreateRW(_iDataSize, _iDataCount, _Data);
		}

		if (m_Mode != _Mode)
		{
			DeleteData();
		}

		m_Mode = _Mode;
		m_RWMode = _RWMode;

		switch (_Mode)
		{
		case BUFFERMODE::LINK:
			m_Data = _Data;
			break;
		case BUFFERMODE::NEW:
			if (m_RWMode == RWBUFFERMODE::CPU)
			{
				m_Data = new char[RBuffer->GetBufferInfo().ByteWidth];
				if (nullptr != _Data)
				{
					memcpy_s(m_Data, _iDataSize * _iDataCount, _Data, _iDataSize * _iDataCount);
				}
			}
			else if (m_RWMode == RWBUFFERMODE::END)
			{
				GJHGameEngineDebug::AssertMsg("else if (m_RWMode == RWBUFFERMODE::END)");
			}
			break;
		default:
			break;
		}

	}

	void GetData(void* _Ptr, size_t _Size)
	{
		if (RBuffer == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (RBuffer == nullptr)");
			return;
		}

		RBuffer->GetData(_Ptr, _Size);

		return;
	}

	void Setting()
	{
		if (m_Mode == BUFFERMODE::END)
		{
			GJHGameEngineDebug::AssertMsg("if (m_Mode ==C BUFFERMODE::END) 스트럭쳐드버퍼를 세팅안했습니다.");
		}

		if (nullptr == RBuffer->GetBuffer())
		{
			GJHGameEngineDebug::AssertMsg("if (nullptr == RBuffer->GetBuffer())");
		}

		(m_Ptr->*SettingFunc)(m_Reg);
	}

};

class CBufferSetter : public ShaderSetter
{
public:
	std::shared_ptr<GJHDirectConstantBuffer> CBuffer;
	GJHDirectConstantBuffer* m_Ptr;
	void(GJHDirectConstantBuffer::* SettingFunc)(int _Reg);
	BUFFERMODE m_Mode;
	void* m_Data;

public:
	void Init()
	{
		m_Ptr = CBuffer.get();

		switch (m_Type)
		{
		case ShaderType::Vertex:
			SettingFunc = &GJHDirectConstantBuffer::VSSetting;
			break;
		case ShaderType::Hull:
			break;
		case ShaderType::Geometry:
			SettingFunc = &GJHDirectConstantBuffer::GSSetting;
			break;
		case ShaderType::Domain:
			break;
		case ShaderType::Pixel:
			SettingFunc = &GJHDirectConstantBuffer::PSSetting;
			break;
		case ShaderType::End:
			break;
		default:
			break;
		}
	}

	void SettingMode(BUFFERMODE _Mode, void* _Data, size_t _Size)
	{
		if (m_Mode != _Mode)
		{
			DeleteData();
		}

		if (CBuffer->GetBufferInfo().ByteWidth < _Size)
		{
			GJHGameEngineDebug::AssertMsg("if (CBuffer->GetBufferInfo().ByteWidth < _Size)");
		}

		m_Mode = _Mode;

		switch (_Mode)
		{
		case BUFFERMODE::LINK:
			m_Data = _Data;
			break;
		case BUFFERMODE::NEW:
			m_Data = new char[CBuffer->GetBufferInfo().ByteWidth];

			memcpy_s(m_Data, _Size, _Data, _Size);

			break;
		default:
			break;
		}
	}

public:
	void Setting()
	{
		if (m_Mode == BUFFERMODE::END)
		{
			GJHGameEngineString Text = Name;
			Text += "if (m_Mode == CBUFFERMODE::END) 상수버퍼를 세팅안했습니다.";

			GJHGameEngineDebug::AssertMsg(Text);
		}
		
		CBuffer->ChangeDataPtr(m_Data);
		(m_Ptr->*SettingFunc)(m_Reg);
	}

public:
	void DeleteData()
	{
		if (m_Mode == BUFFERMODE::NEW)
		{
			if (m_Data != nullptr)
			{
				delete[] reinterpret_cast<char*>(m_Data);
			}
		}
	}

public:
	CBufferSetter() :
		m_Data(nullptr),
		m_Mode(BUFFERMODE::END)
	{

	}
	~CBufferSetter()
	{
		DeleteData();
	}
};


class GJHDirectRenderTarget;
class GJHPlayerBase : std::enable_shared_from_this<GJHPlayerBase>, public GJHUpdaterBase
{
protected:
	std::map<GJHGameEngineString, std::map<ShaderType, TextureSetter>> m_TextureSetter;
	std::map<GJHGameEngineString, std::map<ShaderType, SamplerSetter>> m_SamplerSetter;
	std::map<GJHGameEngineString, std::map<ShaderType, CBufferSetter>> m_CBufferSetter;
	std::map<GJHGameEngineString, std::map<ShaderType, SBufferSetter>> m_SBufferSetter;
	std::map<GJHGameEngineString, std::map<ShaderType, RBufferSetter>> m_RBufferSetter;

	std::vector<TextureSetter*> m_VecTextureSetter;
	std::vector<SamplerSetter*> m_VecSamplerSetter;
	std::vector<CBufferSetter*> m_VecCBufferSetter;
	std::vector<SBufferSetter*> m_VecSBufferSetter;
	std::vector<RBufferSetter*> m_VecRBufferSetter;

public:
	void AllResSetting();
	void AllTextureSetting();
	void AllSamplerSetting();
	void AllCBufferSetting();
	void AllSBufferSetting();
	void AllRBufferSetting();

	void AllTextureReset();

public:
	void SettingDataClear()
	{
		m_VecTextureSetter.clear();
		m_VecSamplerSetter.clear();
		m_VecCBufferSetter.clear();
		m_VecSBufferSetter.clear();
		m_VecRBufferSetter.clear();
		m_SamplerSetter.clear();
		m_TextureSetter.clear();
		m_CBufferSetter.clear();
		m_SBufferSetter.clear();
		m_RBufferSetter.clear();
	}

public:
	bool IsTextureSetting(const GJHGameEngineString& _Name);
	bool IsSamplerSetting(const GJHGameEngineString& _Name);
	bool IsCBufferSetting(const GJHGameEngineString& _Name);
	bool IsSBufferSetting(const GJHGameEngineString& _Name);
	bool IsRBufferSetting(const GJHGameEngineString& _Name);

public:
	void CreateTextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg);
	void CreateSamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg);
	void CreateCBufferSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg, size_t _Size);
	void CreateSBufferSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg);
	void CreateRBufferSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg);

public:
	void TextureSetting(const GJHGameEngineString& _Name, std::shared_ptr<GJHDirectTexture2D> _Res);
	void TextureSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _TexName);
	void TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName);
	void TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectTexture2D> _Res);

	void SamplerSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _SmpName);
	void SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName);
	void SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectSampler> _Res);

	template<typename DataType>
	void CBufferSettingNew(const GJHGameEngineString& _Name, DataType& _NewData)
	{
		static_assert(std::is_same<typename std::remove_pointer<DataType>::type&, DataType&>::value, "It should not be called with a pointer.");

		if (m_CBufferSetter.end() == m_CBufferSetter.find(_Name))
		{
			GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
		}

		for (auto& Setter : m_CBufferSetter[_Name])
		{
			CBufferSettingNew(_Name, Setter.first, _NewData);
		}
	}

	template<typename DataType>
	void CBufferSettingNew(const GJHGameEngineString& _Name, ShaderType _ShaderType, DataType& _NewData)
	{
		static_assert(std::is_same<typename std::remove_pointer<DataType>::type&, DataType&>::value, "It should not be called with a pointer.");

		m_CBufferSetter[_Name][_ShaderType].SettingMode(BUFFERMODE::NEW, &_NewData, sizeof(DataType));
	}

	template<typename DataType>
	void CBufferSettingLink(const GJHGameEngineString& _Name, DataType& _NewData)
	{
		static_assert(std::is_same<typename std::remove_pointer<DataType>::type&, DataType&>::value, "It should not be called with a pointer.");

		if (m_CBufferSetter.end() == m_CBufferSetter.find(_Name))
		{
			GJHGameEngineDebug::AssertMsg((_Name + L"if (m_TextureSetter.end() != m_TextureSetter.find(_Name))").c_str());
		}

		for (auto& Setter : m_CBufferSetter[_Name])
		{
			CBufferSettingLink(_Name, Setter.first, _NewData);
		}
	}

	template<typename DataType>
	void CBufferSettingLink(const GJHGameEngineString& _Name, ShaderType _ShaderType, DataType& _NewData)
	{
		static_assert(std::is_same<typename std::remove_pointer<DataType>::type&, DataType&>::value, "It should not be called with a pointer.");

		m_CBufferSetter[_Name][_ShaderType].SettingMode(BUFFERMODE::LINK, &_NewData, sizeof(DataType));
	}

	template<typename DataType>
	void SBufferSettingNew(const GJHGameEngineString& _Name, ShaderType _ShaderType, void* _Ptr, UINT _iDataSize, UINT _iDataCount)
	{
		m_SBufferSetter[_Name][_ShaderType].SettingMode(BUFFERMODE::NEW, _Ptr, _iDataSize, _iDataCount);
	}

	void SBufferSettingLink(const GJHGameEngineString& _Name, std::shared_ptr<GJHDirectStructuredBuffer> _Buffer)
	{
		if (m_SBufferSetter.find(_Name) == m_SBufferSetter.end())
		{
			GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
		}

		for (auto& Setter : m_SBufferSetter[_Name])
		{
			SBufferSettingLink(_Name, Setter.first, _Buffer);
		}
	}

	template<typename DataType>
	void SBufferSettingLink(const GJHGameEngineString& _Name, std::vector<DataType>& _DataVector)
	{
		SBufferSettingLink(_Name, &_DataVector[0], sizeof(DataType), static_cast<UINT>(_DataVector.size()));
	}

	void SBufferSettingLink(const GJHGameEngineString& _Name, void* _Ptr, UINT _iDataSize, UINT _iDataCount)
	{
		if (m_SBufferSetter.end() == m_SBufferSetter.find(_Name))
		{
			GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
		}

		for (auto& Setter : m_SBufferSetter[_Name])
		{
			SBufferSettingLink(_Name, Setter.first, _Ptr, _iDataSize, _iDataCount);
		}
	}

	void SBufferSettingLink(const GJHGameEngineString& _Name, ShaderType _ShaderType, void* _Ptr, UINT _iDataSize, UINT _iDataCount)
	{
		m_SBufferSetter[_Name][_ShaderType].SettingMode(BUFFERMODE::LINK, _Ptr, _iDataSize, _iDataCount);
	}

	void SBufferSettingLink(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectStructuredBuffer> _Buffer)
	{
		m_SBufferSetter[_Name][_ShaderType].SettingComputeBuffer(_Buffer);
	}

	void RBufferSettingNew(const GJHGameEngineString& _Name, ShaderType _ShaderType, RWBUFFERMODE _Mode, void* _Ptr, UINT _iDataSize, UINT _iDataCount)
	{
		m_RBufferSetter[_Name][_ShaderType].SettingMode(BUFFERMODE::NEW, _Mode, _Ptr, _iDataSize, _iDataCount);
	}


	template<typename DataType>
	void RBufferSettingLink(const GJHGameEngineString& _Name, std::vector<DataType>& _DataVector)
	{
		RBufferSettingLink(_Name, &_DataVector[0], sizeof(DataType), static_cast<UINT>(_DataVector.size()));
	}

	void RBufferSettingLink(const GJHGameEngineString& _Name, RWBUFFERMODE _Mode, void* _Ptr, UINT _iDataSize, UINT _iDataCount)
	{
		if (m_RBufferSetter.end() == m_RBufferSetter.find(_Name))
		{
			GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
		}

		for (auto& Setter : m_RBufferSetter[_Name])
		{
			RBufferSettingLink(_Name, _Mode, Setter.first, _Ptr, _iDataSize, _iDataCount);
		}
	}

	void RBufferSettingLink(const GJHGameEngineString& _Name, RWBUFFERMODE _Mode, ShaderType _ShaderType, void* _Ptr, UINT _iDataSize, UINT _iDataCount)
	{
		m_RBufferSetter[_Name][_ShaderType].SettingMode(BUFFERMODE::LINK, _Mode, _Ptr, _iDataSize, _iDataCount);
	}

	template<typename DataType>
	void GetRBufferData(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::vector<DataType>& _DataVector)
	{
		m_RBufferSetter[_Name][_ShaderType].GetData(&_DataVector[0], static_cast<UINT>(sizeof(DataType) * _DataVector.size()));
	}

	std::shared_ptr<GJHDirectStructuredBuffer> GetRBuffer(const GJHGameEngineString& _Name, ShaderType _ShaderType)
	{
		return m_RBufferSetter[_Name][_ShaderType].RBuffer;
	}

	std::shared_ptr<GJHDirectStructuredBuffer> GetSBuffer(const GJHGameEngineString& _Name, ShaderType _ShaderType)
	{
		return m_SBufferSetter[_Name][_ShaderType].SBuffer;
	}

protected:
	void ShaderSettingDataAutoCreate(GJHDirectShader* _Shader);

public:
	GJHPlayerBase();
	~GJHPlayerBase();

public:
	GJHPlayerBase(const GJHPlayerBase& _Other) = delete;
	GJHPlayerBase(const GJHPlayerBase&& _Other) = delete;

public:
	GJHPlayerBase& operator=(const GJHPlayerBase& _Other) = delete;
	GJHPlayerBase& operator=(const GJHPlayerBase&& _Other) = delete;

public:
	std::vector<std::shared_ptr<GJHDirectTexture2D>> GetTexture(const GJHGameEngineString& _Name);
	std::shared_ptr<GJHDirectTexture2D> GetTexture(const GJHGameEngineString& _Name, ShaderType _ShaderType);
};

