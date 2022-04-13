#include "GJHDirectPixelShader.h"

GJHDirectPixelShader::GJHDirectPixelShader() :
	GJHDirectShader(ShaderType::Pixel),
	m_ShaderPtr(nullptr),
	OutPutPath(OutPutPathType::Forward)
{
	m_ShaderType = ShaderType::Pixel;
}

GJHDirectPixelShader::~GJHDirectPixelShader()
{
	if (m_ShaderPtr != nullptr)
	{
		m_ShaderPtr->Release();
		m_ShaderPtr = nullptr;
	}
}

void GJHDirectPixelShader::Setting()
{
	if (m_ShaderPtr == nullptr)
	{
		return;
	}

	GJHGameEngineDevice::GetContext()->PSSetShader(m_ShaderPtr, nullptr, 0);
}

bool GJHDirectPixelShader::CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow)
{
	m_VersionHigh = _VersionHigh;
	m_VersionLow = _VersionLow;

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	char ArrVersion[200];
	sprintf_s(ArrVersion, "ps_%d_%d", m_VersionHigh, m_VersionLow);
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

	if (GJHGameEngineDevice::GetDevice()->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_ShaderPtr) != S_OK)
	{
		MsgAssert("if (GJHGameEngineDevice::GetDevice()->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_ShaderPtr) != S_OK)");
		return false;
	}

	ResCheck(GetShaderType());

	return true;
}