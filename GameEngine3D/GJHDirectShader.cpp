#include "GJHDirectShader.h"
#include <GJHGameEngineDebug.h>

GJHDirectShader::GJHDirectShader(ShaderType _ShaderType) :
	m_pBlob(nullptr),
	m_pErrBlob(nullptr),
	m_ShaderType(_ShaderType)
{

}

GJHDirectShader::~GJHDirectShader()
{
	if (m_pBlob != nullptr)
	{
		m_pBlob->Release();
		m_pBlob = nullptr;
	}

	if (m_pErrBlob != nullptr)
	{
		m_pErrBlob->Release();
		m_pErrBlob = nullptr;
	}
}

void GJHDirectShader::ResCheck(ShaderType _Type)
{
	ID3D11ShaderReflection* CompileInfo = nullptr;

	if (D3DReflect(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&CompileInfo) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("쉐이더 컴파일 정보를 얻어오지 못했습니다.");
	}

	D3D11_SHADER_DESC Info;
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResInfo;
	ShaderData Data;

	for (unsigned int i = 0; i < Info.BoundResources; i++)
	{
		CompileInfo->GetResourceBindingDesc(i, &ResInfo);

		Data.Name.SetConvertMultiByteString(ResInfo.Name);
		Data.m_ShaderType = m_ShaderType;
		Data.m_Reg = ResInfo.BindPoint;
		Data.m_ResType = ResInfo.Type;

		D3D_SHADER_INPUT_TYPE Type = Data.m_ResType;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* BufferInfo = CompileInfo->GetConstantBufferByName(ResInfo.Name);
			D3D11_SHADER_BUFFER_DESC BufferDesc;
			BufferInfo->GetDesc(&BufferDesc);
			Data.m_Size = BufferDesc.Size;
			break;
		}
		case D3D_SIT_TBUFFER:
			break;
		case D3D_SIT_TEXTURE:
			break;
		case D3D_SIT_SAMPLER:
			break;
		case D3D_SIT_UAV_RWTYPED:
			break;
		case D3D_SIT_STRUCTURED:
			break;
		case D3D_SIT_UAV_RWSTRUCTURED:
			break;
		case D3D_SIT_BYTEADDRESS:
			break;
		case D3D_SIT_UAV_RWBYTEADDRESS:
			break;
		case D3D_SIT_UAV_APPEND_STRUCTURED:
			break;
		case D3D_SIT_UAV_CONSUME_STRUCTURED:
			break;
		case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
			break;
		default:
			break;
		}

		auto Check = m_SettingDataMap.insert(std::map<GJHGameEngineString, ShaderData>::value_type(Data.Name, Data));
		if (Check.second == false)
		{
			GJHGameEngineDebug::AssertMsg("if (Check.second == false)");
		}
	}
}