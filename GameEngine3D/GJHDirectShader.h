#pragma once
#include "GJHResBase.h"
#include "GJHGameEngineDevice.h"
#include <map>

enum class OutPutPathType
{
	Forward,
	Defferd,
};

enum class ShaderType
{
	Vertex,
	Hull,
	Geometry,
	Domain,
	Pixel,
	Compute,
	End
};

class GJHMaterial;
class GJHRenderPlayer;
class GJHGameEngineString;
class ShaderData
{
public:
	GJHGameEngineString Name;
	ShaderType m_ShaderType;
	D3D_SHADER_INPUT_TYPE m_ResType;
	unsigned int m_Reg;
	size_t m_Size;

public:
	ShaderData() :
		m_Size(0)
	{

	}
};

class GJHPlayerBase;
class GJHGameEngineString;
class GJHDirectShader : public GJHResBase
{
private:
	friend GJHMaterial;
	friend GJHPlayerBase;

protected:
	ID3DBlob* m_pBlob;
	ID3DBlob* m_pErrBlob;

	unsigned int m_VersionHigh;
	unsigned int m_VersionLow;
	ShaderType m_ShaderType;

public:
	ShaderType GetShaderType()
	{
		return m_ShaderType;
	}

public:
	GJHDirectShader(ShaderType _ShaderType);
	~GJHDirectShader();

public:
	GJHDirectShader(const GJHDirectShader& _Other) = delete;
	GJHDirectShader(const GJHDirectShader&& _Other) = delete;

public:
	GJHDirectShader operator=(const GJHDirectShader& _Other) = delete;
	GJHDirectShader operator=(const GJHDirectShader&& _Other) = delete;

private:
	std::map<GJHGameEngineString, ShaderData> m_SettingDataMap;

protected:
	void ResCheck(ShaderType _Type);
};