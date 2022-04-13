#include "GJHDirectVertexShader.h"
#include "GJHGameEngineDevice.h"

GJHDirectVertexShader::GJHDirectVertexShader() :
	GJHDirectShader(ShaderType::Vertex),
	m_ShaderPtr(nullptr),
	m_LayOutOffset(0),
	m_pLayOut(nullptr)
{
	m_ShaderType = ShaderType::Vertex;
}

GJHDirectVertexShader::~GJHDirectVertexShader()
{
	if (m_ShaderPtr != nullptr)
	{
		m_ShaderPtr->Release();
		m_ShaderPtr = nullptr;
	}

	if (m_pLayOut != nullptr)
	{
		m_pLayOut->Release();
		m_pLayOut = nullptr;
	}
}

void GJHDirectVertexShader::Setting()
{
	if (m_ShaderPtr == nullptr)
	{
		return;
	}

	if (m_pLayOut != nullptr)
	{
		GJHGameEngineDevice::GetContext()->IASetInputLayout(m_pLayOut);
	}

	GJHGameEngineDevice::GetContext()->VSSetShader(m_ShaderPtr, nullptr, 0);
}

bool GJHDirectVertexShader::CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow)
{
	m_VersionHigh = _VersionHigh;
	m_VersionLow = _VersionLow;

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	char ArrVersion[200];
	sprintf_s(ArrVersion, "vs_%d_%d", m_VersionHigh, m_VersionLow);
	std::string FuncName = _FunctionName.GetConvertMultiByteString();

	if (D3DCompileFromFile(_Path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		FuncName.c_str(),
		ArrVersion,
		Flag,
		0,
		&m_pBlob,
		&m_pErrBlob) != S_OK)
	{
		std::string Error = (char*)m_pErrBlob->GetBufferPointer();
		MsgAssert(Error.c_str());
		return false;
	}

	if (GJHGameEngineDevice::GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_ShaderPtr) != S_OK)
	{
		MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_ShaderPtr) != S_OK)");
		return false;
	}

	LayOutCheck();
	ResCheck(GetShaderType());

	return true;
}

void GJHDirectVertexShader::AddLayOut(
	const char* _SmtName,
	unsigned int _Index,
	DXGI_FORMAT _Format,
	unsigned int _inputSlot,
	unsigned int _idsr,
	D3D11_INPUT_CLASSIFICATION _inputClass)
{
	m_SmtName.push_back(_SmtName);

	D3D11_INPUT_ELEMENT_DESC LayOutInfo = { 0, };
	LayOutInfo.SemanticName = _SmtName;
	LayOutInfo.SemanticIndex = (unsigned int)_Index;
	LayOutInfo.Format = _Format;
	LayOutInfo.InputSlot = _inputSlot;
	LayOutInfo.InstanceDataStepRate = _idsr;
	LayOutInfo.InputSlotClass = _inputClass;
	LayOutInfo.AlignedByteOffset = m_LayOutOffset;

	m_LayOutOffset += GJHGameEngineDevice::FormatSize(_Format);
	m_LayOutInfoVector.push_back(LayOutInfo);
}

void GJHDirectVertexShader::CreateLayOut()
{
	if (GJHGameEngineDevice::GetDevice()->CreateInputLayout(
		&m_LayOutInfoVector[0],
		(unsigned int)m_LayOutInfoVector.size(),
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		&m_pLayOut) != S_OK)
	{
		MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreateInputLayout");
	}
}

void GJHDirectVertexShader::LayOutCheck()
{
	ID3D11ShaderReflection* CompileInfo = nullptr;

	if (D3DReflect(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&CompileInfo) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("쉐이더 컴파일 정보를 얻어오지 못했습니다.");
	}

	D3D11_SHADER_DESC Info;

	CompileInfo->GetDesc(&Info);

	for (unsigned int i = 0; i < Info.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC Input = { };
		CompileInfo->GetInputParameterDesc(i, &Input);

		DXGI_FORMAT FormatType = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

		D3D_REGISTER_COMPONENT_TYPE Reg = Input.ComponentType;

		switch (Input.Mask)
		{
		case 1:
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
				break;
			default:
				break;
			}
			break;
		case 3:
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
				break;
			default:
				break;
			}

			break;
		case 7:
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			default:
				break;
			}

			break;
		case 15:
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				// unsigned int형 정보라 뜻
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				FormatType = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		if (FormatType == DXGI_FORMAT::DXGI_FORMAT_UNKNOWN)
		{
			GJHGameEngineDebug::AssertMsg("if (FormatType == DXGI_FORMAT::DXGI_FORMAT_UNKNOWN)");
		}

		AddLayOut(Input.SemanticName, Input.SemanticIndex, FormatType);
	}

	CreateLayOut();
}