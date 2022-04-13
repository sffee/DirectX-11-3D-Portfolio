#pragma once
#include "GJHPlayerBase.h"
#include "GJHDirectMesh.h"
#include "GJHMaterial.h"
#include "GJHDirectMesh.h"
#include <functional>

enum class RENDERINGTYPE
{
	NORMAL,
	INSTANCE,
};

class GJHMaterial;
class GJHDirectRenderTarget;
class GJHRenderPlayer : public GJHPlayerBase
{
private:
public:
private:
	std::shared_ptr<GJHMaterial> m_Material;
	std::shared_ptr<GJHDirectMesh> m_Mesh;

	RENDERINGTYPE m_RenderType;
	int m_InstanceCount;

public:
	std::shared_ptr<GJHDirectMesh> GetMesh()
	{
		return m_Mesh;
	}

	void SetRenderType(RENDERINGTYPE _RenderType)
	{
		m_RenderType = _RenderType;
	}

	void SetInstanceCount(int _InstanceCount)
	{
		m_InstanceCount = _InstanceCount;
	}

public:
	OutPutPathType GetOutPutPath();

	void SetVertexShader(const GJHGameEngineString& _Name);
	void SetGeometryShader(const GJHGameEngineString& _Name);
	void SetPixelShader(const GJHGameEngineString& _Name);
	void SetRasterizer(const GJHGameEngineString& _Name);
	void SetDepthStencil(const GJHGameEngineString& _Name);
	void SetBlend(const GJHGameEngineString& _Name);

public:
	void ReLoadingSettingData();

	void SetMaterial(const GJHGameEngineString& _Name, std::function<void()> _ResultPtr = nullptr);
	void SetMaterial(std::shared_ptr<GJHMaterial> _Mat, std::function<void()> _ResultPtr = nullptr);
	void SetMesh(const GJHGameEngineString& _Name);
	void SetMesh(std::shared_ptr<GJHDirectMesh> _Mesh);

public:
	GJHRenderPlayer();
	~GJHRenderPlayer();

public:
	GJHRenderPlayer(const GJHRenderPlayer& _Other) = delete;
	GJHRenderPlayer(const GJHRenderPlayer&& _Other) = delete;

public:
	GJHRenderPlayer& operator=(const GJHRenderPlayer& _Other) = delete;
	GJHRenderPlayer& operator=(const GJHRenderPlayer&& _Other) = delete;

public:
	void Render();
	void Render(std::shared_ptr<GJHDirectRenderTarget> _Target);
};

