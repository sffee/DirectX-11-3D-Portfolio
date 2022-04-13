#pragma once
#include "GJHDirectResourcesManager.h"
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include <GJHGameEnginePath.h>
#include "GJHDirectShader.h"
#include <vector>

class GJHDirectComputeShader : public GJHDirectShader, public GJHDirectResourcesManager<GJHDirectComputeShader>
{
private:
public:
    static std::shared_ptr<GJHDirectComputeShader> Create(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh = 5, int _VersionLow = 0)
    {
        GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
        GJHDirectComputeShader* NewRes = new GJHDirectComputeShader();
        if (false == NewRes->CreateShader(_Path, _FunctionName, _VersionHigh, _VersionLow))
        {
            return nullptr;
        }
        std::shared_ptr<GJHDirectComputeShader> ReturnPtr = InsertRes(FileName, NewRes);
        return ReturnPtr;
    }


private:
    ID3D11ComputeShader* m_ShaderPtr;

public:
    GJHDirectComputeShader();
    ~GJHDirectComputeShader();

public:
    GJHDirectComputeShader(const GJHDirectComputeShader& _Other) = delete;
    GJHDirectComputeShader(const GJHDirectComputeShader&& _Other) = delete;

public:
    GJHDirectComputeShader& operator=(const GJHDirectComputeShader& _Other) = delete;
    GJHDirectComputeShader& operator=(const GJHDirectComputeShader&& _Other) = delete;

public:
    bool CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow);
    void Setting() override;
    void Compute(UINT _X, UINT _Y, UINT _Z);
};

