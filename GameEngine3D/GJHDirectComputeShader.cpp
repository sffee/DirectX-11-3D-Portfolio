#include "GJHDirectComputeShader.h"

GJHDirectComputeShader::GJHDirectComputeShader() : GJHDirectShader(ShaderType::Compute), m_ShaderPtr(nullptr)
{
}

GJHDirectComputeShader::~GJHDirectComputeShader()
{
	if (m_ShaderPtr != nullptr)
	{
		m_ShaderPtr->Release();
		m_ShaderPtr = nullptr;
	}
}

bool GJHDirectComputeShader::CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow)
{
	m_VersionHigh = _VersionHigh;
	m_VersionLow = _VersionLow;

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	char ArrVersion[200];
	sprintf_s(ArrVersion, "cs_%d_%d", m_VersionHigh, m_VersionLow);
	std::string FuncName = _FunctionName.GetConvertMultiByteString();

	if (S_OK != D3DCompileFromFile(_Path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		FuncName.c_str(),
		ArrVersion,
		Flag,
		0,
		&m_pBlob,
		&m_pErrBlob))
	{
		std::string Error = (char*)m_pErrBlob->GetBufferPointer();
		GJHGameEngineDebug::AssertMsg(Error.c_str());
		return false;
	}

	if (S_OK != GJHGameEngineDevice::GetDevice()->CreateComputeShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_ShaderPtr))
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineDevice::GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_ShaderPtr))");
		return false;
	}

	ResCheck(GetShaderType());

	return true;
}
void GJHDirectComputeShader::Setting()
{
	GJHGameEngineDevice::GetContext()->CSSetShader(m_ShaderPtr, nullptr, 0);
}

void GJHDirectComputeShader::Compute(UINT _X, UINT _Y, UINT _Z)
{
	GJHGameEngineDevice::GetContext()->Dispatch(_X, _Y, _Z);
}