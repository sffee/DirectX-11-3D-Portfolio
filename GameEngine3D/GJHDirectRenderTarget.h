#pragma once
#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHResBase.h"
#include "GJHRenderPlayer.h"

class GJHDirectTexture2D;
class GJHGameEngineDevice;
class GJHDirectRenderTarget : public GJHResBase
{
private:
	friend GJHGameEngineDevice;

private:
	static GJHRenderPlayer CopyPlayer;

private:
	static void Init();

private:
	std::vector<std::shared_ptr<GJHDirectTexture2D>> m_pArrTarget;
	
	std::vector<ID3D11RenderTargetView*> m_pArrTargetView;
	std::vector<D3D11_VIEWPORT> m_ArrViewPort;

	std::vector<float4> m_pArrClearColor;
	std::shared_ptr<GJHDirectTexture2D> m_pDepthTarget;

	unsigned int m_SettingIndex;

public:
	size_t GetTexsize()
	{
		return m_pArrTarget.size();
	}

	std::shared_ptr<GJHDirectTexture2D> GetTex(int _Index)
	{
		return m_pArrTarget[_Index];
	}

public:
	std::shared_ptr<GJHDirectTexture2D> GetDepthTarget()
	{
		return m_pDepthTarget;
	}

	void SetDepthTarget(std::shared_ptr<GJHDirectTexture2D> _Depth)
	{
		m_pDepthTarget = _Depth;
	}

	std::shared_ptr<GJHDirectTexture2D> GetDirectTexture2D()
	{
		return nullptr;
	}

public:
	void CreateRenderTarget(float4 _Size, float4 _ClearColor);
	void CreateRenderTarget(float4 _Size, DXGI_FORMAT _Format, float4 _ClearColor);
	void CreateRenderTarget(ID3D11Texture2D* _pTexture2D, float4 _ClearColor);
	void CreateRenderTarget(std::shared_ptr<GJHDirectTexture2D> _pTexture2D, float4 _ClearColor);
	void CreateDepthTarget(float4 _Size);

public:
	GJHDirectRenderTarget();
	~GJHDirectRenderTarget();
	GJHDirectRenderTarget(const GJHDirectRenderTarget& _Other) = delete;
	GJHDirectRenderTarget(const GJHDirectRenderTarget&& _Other) = delete;

public:
	GJHDirectRenderTarget operator=(const GJHDirectRenderTarget& _Other) = delete;
	GJHDirectRenderTarget operator=(const GJHDirectRenderTarget&& _Other) = delete;

public:
	void Clear();
	void Setting();

	void Copy(std::shared_ptr<GJHDirectRenderTarget> _OtherTarget);
	void Merge(std::shared_ptr<GJHDirectRenderTarget> _OtherTarget);
};