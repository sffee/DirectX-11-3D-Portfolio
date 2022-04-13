#pragma once
#include "GJHDirectShader.h"
#include "GJHDirectResourcesManager.h"
#include <GJHGameEnginePath.h>

class GJHDirectPixelShader : public GJHDirectShader, public GJHDirectResourcesManager<GJHDirectPixelShader>
{
public:
	static std::shared_ptr<GJHDirectPixelShader> Create(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh = 5, int _VersionLow = 0)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		
		std::shared_ptr<GJHDirectPixelShader> NewRes = std::make_shared<GJHDirectPixelShader>();

		if (NewRes->CreateShader(_Path, _FunctionName, _VersionHigh, _VersionLow) == false)
		{
			return NewRes;
		}

		InsertResSheard(FileName, NewRes);

		return NewRes;
	}

private:
	ID3D11PixelShader* m_ShaderPtr;
	OutPutPathType OutPutPath;

public:
	OutPutPathType GetOutPutPathType()
	{
		return OutPutPath;
	}

	void SetOutPutPath(OutPutPathType _OutPutPath)
	{
		OutPutPath = _OutPutPath;
	}

public:
	GJHDirectPixelShader();
	~GJHDirectPixelShader();

public:
	GJHDirectPixelShader(const GJHDirectPixelShader& _Other) = delete;
	GJHDirectPixelShader(const GJHDirectPixelShader&& _Other) = delete;

public:
	GJHDirectPixelShader operator=(const GJHDirectPixelShader& _Other) = delete;
	GJHDirectPixelShader operator=(const GJHDirectPixelShader&& _Other) = delete;

public:
	bool CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow);
	void Setting() override;
};