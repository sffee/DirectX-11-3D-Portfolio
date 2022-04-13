#pragma once
#include "GJHGameEngineDevice.h"
#include "GJHResBase.h"
#include <GJHGameEngineString.h>
#include "GJHDirectResourcesManager.h"
#include <GJHGameEnginePath.h>
#include "DirectXTex.h"
#include "GJHDirectShader.h"

#pragma comment(lib, "DirectXTex")

class GJHDirectTexture2D : public GJHResBase, public GJHDirectResourcesManager<GJHDirectTexture2D>
{
public:
	static std::shared_ptr<GJHDirectTexture2D> Load(const GJHGameEngineString& _Path)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		if (Find(FileName) != nullptr)
		{
			return Find(FileName);
		}

		GJHDirectTexture2D* NewRes = new GJHDirectTexture2D();

		if (NewRes->LoadRes(_Path, FileName) == false)
		{
			return nullptr;
		}

		std::shared_ptr<GJHDirectTexture2D> ReturnPtr = InsertRes(FileName, NewRes);

		return ReturnPtr;
	}

private:
	DirectX::ScratchImage m_Image;

	D3D11_TEXTURE2D_DESC m_Info;
	ID3D11Texture2D* m_pTexture2D;
	ID3D11RenderTargetView* m_pRTV;
	ID3D11DepthStencilView* m_pDSV;
	ID3D11ShaderResourceView* m_pSRV;

	GJHGameEngineString m_TextureName;

public:
	ID3D11Texture2D* GetTexture2D() 
	{
		return m_pTexture2D;
	}

public:
	float4 GetSize()
	{
		return float4({ (float)m_Info.Width, (float)m_Info.Height });
	}

	unsigned int GetSizeX()
	{
		return m_Info.Width;
	}

	unsigned int GetSizeY()
	{
		return m_Info.Height;
	}

	unsigned __int64 GetSizeX64()
	{
		return static_cast<unsigned __int64>(m_Info.Width);
	}

	unsigned __int64 GetSizeY64()
	{
		return static_cast<unsigned __int64>(m_Info.Height);
	}

	GJHGameEngineString GetTextureName()
	{
		return m_TextureName;
	}

public:
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_pRTV;
	}

	ID3D11DepthStencilView* GetDepthStencilView()
	{
		return m_pDSV;
	}

	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return m_pSRV;
	}

public:
	GJHDirectTexture2D();
	~GJHDirectTexture2D();

private:
	void CreateTexture2D(const D3D11_TEXTURE2D_DESC& _Desc);
	void CreateRes(const int _BindFlag);

public:
	template<typename ... REST>
	void Create(ID3D11Texture2D* _pTexture2D, const int _BindFlag)
	{
		m_pTexture2D = _pTexture2D;
		if (m_pTexture2D != nullptr)
		{
			m_pTexture2D->GetDesc(&m_Info);
		}

		CreateRes(_BindFlag);
	}

	template<typename ... REST>
	void Create(const float4& _Size, DXGI_FORMAT _Fmt, const int _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		D3D11_TEXTURE2D_DESC DepthTextureInfo = { 0, };
		DepthTextureInfo.ArraySize = 1;
		DepthTextureInfo.Width = _Size.uix();
		DepthTextureInfo.Height = _Size.uiy();
		DepthTextureInfo.Format = _Fmt;
		DepthTextureInfo.SampleDesc.Count = 1;
		DepthTextureInfo.SampleDesc.Quality = 0;
		DepthTextureInfo.MipLevels = 1;
		DepthTextureInfo.Usage = _Usage;

		if (_Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
		{
			DepthTextureInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			DepthTextureInfo.CPUAccessFlags = 0;
		}

		DepthTextureInfo.BindFlags = _BindFlag;

		CreateTexture2D(DepthTextureInfo);
		CreateRes(_BindFlag);
	}

	template<typename ... REST>
	void Create(const D3D11_TEXTURE2D_DESC& _Desc)
	{
		CreateTexture2D(_Desc);
		CreateRes(_Desc.BindFlags);
	}

	bool LoadRes(const GJHGameEngineString& _Path, const GJHGameEngineString& _FileName);

public:
	void Setting(ShaderType _Type, int _Reg);
	void VSSetting(int _Reg);
	void GSSetting(int _Reg);
	void PSSetting(int _Reg);

	void VSReset(int _Reg);
	void PSReset(int _Reg);
	void GSReset(int _Reg);

	void CaptureTexture();
	void SavePngFile(const GJHGameEngineString& _FileName);

public:
	GJHDirectTexture2D(const GJHDirectTexture2D& _Other) = delete;
	GJHDirectTexture2D(const GJHDirectTexture2D&& _Other) = delete;

public:
	GJHDirectTexture2D operator=(const GJHDirectTexture2D& _Other) = delete;
	GJHDirectTexture2D operator=(const GJHDirectTexture2D&& _Other) = delete;

public:
	float4 GetPixel(int _X, int _Y);
	Color255 GetPixel255(int _X, int _Y);
	void SetPixel(void* _PixelData, int Size);
};

typedef std::shared_ptr<GJHDirectTexture2D> TexPtr;