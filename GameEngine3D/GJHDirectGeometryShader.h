#pragma once
#include "GJHDirectResourcesManager.h"
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include <GJHGameEnginePath.h>
#include "GJHDirectShader.h"
#include <vector>

class GJHDirectGeometryShader : public GJHDirectShader, public GJHDirectResourcesManager<GJHDirectGeometryShader>
{
public:
	static std::shared_ptr<GJHDirectGeometryShader> Create(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh = 5, int _VersionLow = 0)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		GJHDirectGeometryShader* NewRes = new GJHDirectGeometryShader();

		if (NewRes->CreateShader(_Path, _FunctionName, _VersionHigh, _VersionLow) == false)
		{
			return nullptr;
		}

		std::shared_ptr<GJHDirectGeometryShader> ReturnPtr = InsertRes(FileName, NewRes);

		return ReturnPtr;
	}

private:
	ID3D11GeometryShader* m_ShaderPtr;

public:
	GJHDirectGeometryShader();
	~GJHDirectGeometryShader();

public:
	GJHDirectGeometryShader(const GJHDirectGeometryShader& _Other) = delete;
	GJHDirectGeometryShader(const GJHDirectGeometryShader&& _Other) = delete;

public:
	GJHDirectGeometryShader operator=(const GJHDirectGeometryShader& _Other) = delete;
	GJHDirectGeometryShader operator=(const GJHDirectGeometryShader&& _Other) = delete;

public:
	bool CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow);
	void Setting() override;
};