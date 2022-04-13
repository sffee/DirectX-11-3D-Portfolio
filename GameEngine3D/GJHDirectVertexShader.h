#pragma once
#include "GJHDirectResourcesManager.h"
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include <GJHGameEnginePath.h>
#include "GJHDirectShader.h"
#include <vector>

class GJHDirectVertexShader : public GJHDirectShader, public GJHDirectResourcesManager<GJHDirectVertexShader>
{
public:
	static std::shared_ptr<GJHDirectVertexShader> Create(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh = 5, int _VersionLow = 0)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		GJHDirectVertexShader* NewRes = new GJHDirectVertexShader();

		if (NewRes->CreateShader(_Path, _FunctionName, _VersionHigh, _VersionLow) == false)
		{
			return nullptr;
		}

		std::shared_ptr<GJHDirectVertexShader> ReturnPtr = InsertRes(FileName, NewRes);
		
		return ReturnPtr;
	}

private:
	ID3D11VertexShader* m_ShaderPtr;

public:
	GJHDirectVertexShader();
	~GJHDirectVertexShader();
	GJHDirectVertexShader(const GJHDirectVertexShader& _Other) = delete;
	GJHDirectVertexShader(const GJHDirectVertexShader&& _Other) = delete;

public:
	GJHDirectVertexShader operator=(const GJHDirectVertexShader& _Other) = delete;
	GJHDirectVertexShader operator=(const GJHDirectVertexShader&& _Other) = delete;

public:
	bool CreateShader(const GJHGameEngineString& _Path, const GJHGameEngineString& _FunctionName, int _VersionHigh, int _VersionLow);
	void Setting() override;

private:
	ID3D11InputLayout* m_pLayOut;
	unsigned int m_LayOutOffset;
	std::vector<std::string> m_SmtName;
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_LayOutInfoVector;

public:
	void LayOutCheck();

	void AddLayOut(
		const char* _SmtName,
		unsigned int _Index,
		DXGI_FORMAT _Format,
		unsigned int _inputSlot = 0,
		unsigned int _idsr = 0,
		D3D11_INPUT_CLASSIFICATION _inputClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA
	);

	void CreateLayOut();
};