#pragma once
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <GJHGameEngineMath.h>
#include <memory>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

class GJHDirectRenderTarget;
class GJHGameEngineDevice
{
private:
	static uint64_t m_PresentCount;
	static UINT m_MC;
	static UINT m_MQ;
	static float4 m_Size;
	static HWND m_Hwnd;
	static ID3D11Device* m_pDevice;
	static ID3D11DeviceContext* m_pContext;
	static IDXGISwapChain* m_pSwapChain;

	static std::shared_ptr<GJHDirectRenderTarget> m_BackBuffer;

public:
	static void Reset();
	static void VtxShaderReset();
	static void PixShaderReset();
	static void GeoShaderReset();
	static void ComShaderReset();
	static void RasterizerReset();
	static void DepthStencilReset();
	static void BlendReset();
	static void ShaderResReset();

public:
	static unsigned int FormatSize(DXGI_FORMAT _Format);

	static ID3D11Device* GetDevice()
	{
		return m_pDevice;
	}

	static IDXGISwapChain* GetSwapChain()
	{
		return m_pSwapChain;
	}

	static ID3D11DeviceContext* GetContext()
	{
		return m_pContext;
	}

	static std::shared_ptr<GJHDirectRenderTarget> GetBackBuffer()
	{
		return m_BackBuffer;
	}

private:
	static void CreateSwapChain();
	static void CreateDeviceRenderTarget();

public:
	static void Init(HWND _Hwnd, float4 _Size);
	static void Release();
	static void BackBufferSetting();
	static void Present();
	static void ClearRenderingPipeLine();

private:
	static void ResourceLoading();

private:
	static void MeshInit();
	static void ShaderInit();
	static void RasterizerInit();
	static void DepthStencilInit();
	static void SamplerInit();
	static void BlendInit();
	static void MaterialInit();

public:
	GJHGameEngineDevice();
	~GJHGameEngineDevice();
	GJHGameEngineDevice(const GJHGameEngineDevice& _Other) = delete;
	GJHGameEngineDevice(const GJHGameEngineDevice&& _Other) = delete;

public:
	GJHGameEngineDevice operator=(const GJHGameEngineDevice& _Other) = delete;
	GJHGameEngineDevice operator=(const GJHGameEngineDevice&& _Other) = delete;
};