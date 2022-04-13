#include "GJHDirectRenderTarget.h"
#include "GJHDirectTexture2D.h"
#include "GJHGameEngineDevice.h"
#include <functional>

GJHRenderPlayer GJHDirectRenderTarget::CopyPlayer;

void GJHDirectRenderTarget::Init()
{
	CopyPlayer.SetMaterial(L"TargetCopyMaterial");
	CopyPlayer.SetMesh(L"FullRect");
	CopyPlayer.SamplerSetting(L"Smp", L"SMPLINEAR");
}

GJHDirectRenderTarget::GJHDirectRenderTarget() :
	m_SettingIndex(0)
{

}

GJHDirectRenderTarget::~GJHDirectRenderTarget()
{
	if (m_pDepthTarget != nullptr)
	{
		m_pDepthTarget = nullptr;
	}

	for (size_t i = 0; i < m_pArrTarget.size(); i++)
	{
		m_pArrTarget[i] = nullptr;
	}
}

void GJHDirectRenderTarget::CreateRenderTarget(float4 _Size, float4 _ClearColor)
{
	std::shared_ptr<GJHDirectTexture2D> NewPtr = std::make_shared<GJHDirectTexture2D>();
	NewPtr->Create(_Size, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	CreateRenderTarget(NewPtr, _ClearColor);
}

void GJHDirectRenderTarget::CreateRenderTarget(float4 _Size, DXGI_FORMAT _Format, float4 _ClearColor)
{
	std::shared_ptr<GJHDirectTexture2D> NewPtr = std::make_shared<GJHDirectTexture2D>();
	NewPtr->Create(_Size, _Format, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	CreateRenderTarget(NewPtr, _ClearColor);
}

void GJHDirectRenderTarget::CreateRenderTarget(ID3D11Texture2D* _pTexture2D, float4 _ClearColor)
{
	std::shared_ptr<GJHDirectTexture2D> NewPtr = std::make_shared<GJHDirectTexture2D>();
	NewPtr->Create(_pTexture2D, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	CreateRenderTarget(NewPtr, _ClearColor);
}

void GJHDirectRenderTarget::CreateRenderTarget(std::shared_ptr<GJHDirectTexture2D> _pTexture2D, float4 _ClearColor)
{
	m_pArrTarget.push_back(_pTexture2D);
	m_pArrClearColor.push_back(_ClearColor);
	m_pArrTargetView.push_back(_pTexture2D->GetRenderTargetView());
	m_ArrViewPort.push_back(D3D11_VIEWPORT{ 0, 0, (float)_pTexture2D->GetSizeX(), (float)_pTexture2D->GetSizeY(), 0.f, 1.f });
}

void GJHDirectRenderTarget::CreateDepthTarget(float4 _Size)
{
	m_pDepthTarget = std::make_shared<GJHDirectTexture2D>();

	D3D11_TEXTURE2D_DESC DepthTextureInfo = { 0, };

	DepthTextureInfo.ArraySize = 1;
	DepthTextureInfo.Width = _Size.uix();
	DepthTextureInfo.Height = _Size.uiy();

	DepthTextureInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthTextureInfo.SampleDesc.Count = 1;
	DepthTextureInfo.SampleDesc.Quality = 0;

	DepthTextureInfo.MipLevels = 1;

	DepthTextureInfo.Usage = D3D11_USAGE_DEFAULT;

	DepthTextureInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDepthTarget->Create(DepthTextureInfo);
}

void GJHDirectRenderTarget::Clear()
{
	if (m_pDepthTarget != nullptr)
	{
		ID3D11DepthStencilView* DSV = m_pDepthTarget->GetDepthStencilView();
		GJHGameEngineDevice::GetContext()->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	for (size_t i = 0; i < m_pArrTarget.size(); i++)
	{
		if (m_pArrTarget[i] == nullptr)
		{
			continue;
		}

		ID3D11RenderTargetView* RTV = m_pArrTarget[i]->GetRenderTargetView();

		GJHGameEngineDevice::GetContext()->ClearRenderTargetView(RTV, m_pArrClearColor[i].Arr);
	}
}

void GJHDirectRenderTarget::Setting()
{
	if (m_pArrTargetView[m_SettingIndex] != nullptr)
	{
		GJHGameEngineDevice::GetContext()->RSSetViewports((int)m_ArrViewPort.size(), &m_ArrViewPort[m_SettingIndex]);

		if (m_pDepthTarget != nullptr)
		{
			GJHGameEngineDevice::GetContext()->OMSetRenderTargets((int)m_pArrTargetView.size(), &m_pArrTargetView[0], m_pDepthTarget->GetDepthStencilView());
		}
		else
		{
			GJHGameEngineDevice::GetContext()->OMSetRenderTargets((int)m_pArrTargetView.size(), &m_pArrTargetView[0], nullptr);
		}
	}
}

void GJHDirectRenderTarget::Copy(std::shared_ptr<GJHDirectRenderTarget> _OtherTarget)
{
	GJHGameEngineDevice::Reset();

	Clear();
	Merge(_OtherTarget);
}

void GJHDirectRenderTarget::Merge(std::shared_ptr<GJHDirectRenderTarget> _OtherTarget)
{
	GJHGameEngineDevice::Reset();

	Setting();
	CopyPlayer.TextureSetting(L"Tex", ShaderType::Pixel, _OtherTarget->GetTex(0));
	CopyPlayer.Render();
}