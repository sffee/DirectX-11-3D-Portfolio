#pragma once
#include "GJHActorTransform.h"
#include <GJHGameEngineMath.h>
#include <GJHRenderPlayer.h>
#include <GJHLightData.h>

struct RenderOption
{
	int bBump;
	int bTemp0;
	int bTemp1;
	int bTemp2;

	RenderOption()
		: bBump(0)
	{

	}
};

class GJHLight;
class LightSetData;
class GJHRenderManagerComponent;
class GJHRenderer : public GJHActorTransform
{
private:
	friend GJHRenderManagerComponent;

public:
	RenderOption Option;

private:
	int m_Order;
	bool m_ChangeOrder;
	bool m_ShadowOn;
	bool m_AniOn;

public:
	bool GetIsAni()
	{
		return m_AniOn;
	}

	void SetAniOn()
	{
		m_AniOn = true;
	}

	void SetAniOff()
	{
		m_AniOn = false;
	}

	bool GetIsShadow()
	{
		return m_ShadowOn;
	}

	void SetShadowOn()
	{
		m_ShadowOn = true;
	}

	void SetShadowOff()
	{
		m_ShadowOn = false;
	}

protected:
	GJHLight* CurLight;
	GJHCamera* m_CurCamera;
	std::shared_ptr<GJHDirectStructuredBuffer> m_pLightStructuredBuffer;

	LightSetData* m_pLightSetData;

private:
	GJHActorTransform* m_pActorTransform;

protected:
	std::vector<std::shared_ptr<GJHRenderPlayer>> m_PlayerList;

public:
	void SetOrder(int _Order)
	{
		m_Order = _Order;
		m_ChangeOrder = true;
	}

	int GetOrder()
	{
		return m_Order;
	}

public:
	GJHRenderer();
	~GJHRenderer();

public:
	GJHRenderer(const GJHRenderer& _Other) = delete;
	GJHRenderer(const GJHRenderer&& _Other) = delete;

public:
	GJHRenderer operator=(const GJHRenderer& _Other) = delete;
	GJHRenderer operator=(const GJHRenderer&& _Other) = delete;

public:
	std::shared_ptr<GJHRenderPlayer> CreateRenderPlayer(std::shared_ptr<GJHDirectMesh> _Mesh);
	std::shared_ptr<GJHRenderPlayer> CreateRenderPlayer(const GJHGameEngineString& _Material, std::shared_ptr<GJHDirectMesh> _Mesh);
	std::shared_ptr<GJHRenderPlayer> CreateRenderPlayer(const GJHGameEngineString& _Material, const GJHGameEngineString& _Mesh);
	std::shared_ptr<GJHRenderPlayer> GetRenderPlayer(size_t _Index);

	void SetMaterial(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetVertexShader(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetGeometryShader(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetPixelShader(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetRasterizer(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetDepthStencil(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetBlend(const GJHGameEngineString& _Name, size_t _Index = 0);
	void SetMesh(const GJHGameEngineString& _Name, size_t _Index = 0);

public:
	void Start(int _Order = 0);
	void Start(int _Order, const GJHGameEngineString& _Material, const GJHGameEngineString& _Mesh);
	virtual void RenderingSetting();
	virtual void LightRenderingSetting();

public:
	void TextureSetting(const GJHGameEngineString& _Name, std::shared_ptr<GJHDirectTexture2D> _Res, bool _AutoSize = false, int _RenderPlayerIndex = 0);
	void TextureSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _TexName, bool _AutoSize = false, int _RenderPlayerIndex = 0);
	void TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName, bool _AutoSize = false, int _RenderPlayerIndex = 0);
	void TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectTexture2D> _Res, bool _AutoSize = false, int _RenderPlayerIndex = 0);

	void SamplerSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _SmpName, int _RenderPlayerIndex = 0);
	void SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _SmpName, int _RenderPlayerIndex = 0);
	void SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectSampler> _Res, int _RenderPlayerIndex = 0);

	template<typename DataType>
	void CBufferSettingNew(const GJHGameEngineString& _Name, DataType& _NewData, int _RenderPlayerIndex = 0)
	{
		m_PlayerList[_RenderPlayerIndex]->CBufferSettingNew(_Name, _NewData);
	}

	template<typename DataType>
	void CBufferSettingNew(const GJHGameEngineString& _Name, ShaderType _ShaderType, DataType& _NewData, int _RenderPlayerIndex = 0)
	{
		m_PlayerList[_RenderPlayerIndex]->CBufferSettingNew(_Name, _ShaderType, _NewData);
	}

	template<typename DataType>
	void CBufferSettingLink(const GJHGameEngineString& _Name, DataType& _NewData, int _RenderPlayerIndex = 0)
	{
		static_assert(std::is_same<typename std::remove_pointer<DataType>::type&, DataType&>::value, "It should not be called with a pointer.");

		m_PlayerList[_RenderPlayerIndex]->CBufferSettingLink(_Name, _NewData);
	}

	template<typename DataType>
	void CBufferSettingLink(const GJHGameEngineString& _Name, ShaderType _ShaderType, DataType& _NewData, int _RenderPlayerIndex = 0)
	{
		static_assert(std::is_same<typename std::remove_pointer<DataType>::type&, DataType&>::value, "It should not be called with a pointer.");

		m_PlayerList[_RenderPlayerIndex]->CBufferSettingLink(_Name, _ShaderType, _NewData);
	}

public:
	std::vector<std::shared_ptr<GJHDirectTexture2D>> GetTexture(const GJHGameEngineString& _Name, int _RenderPlayerIndex = 0);
	std::shared_ptr<GJHDirectTexture2D> GetTexture(const GJHGameEngineString& _Name = L"Tex", ShaderType _ShaderType = ShaderType::Pixel, int _RenderPlayerIndex = 0);

public:
	std::vector<std::shared_ptr<GJHRenderPlayer>> SetFbxToCreateAllRenderPlayer(const GJHGameEngineString& _Name, const GJHGameEngineString& _MatName, int _Order, std::function<void(GJHRenderPlayer*)> _ResultPtr);
};

using RendererPtr = std::shared_ptr<GJHRenderer>;