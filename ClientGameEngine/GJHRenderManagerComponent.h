#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineTransform.h>
#include <map>
#include <vector>
#include <GJHGameEngineDevice.h>
#include <GJHLightData.h>
#include <GJHDirectShader.h>

enum class PostEffectOrder
{
	CamMergePrev = 5
};

class LightSetData
{
public:
	int LightSetCount;
	int LightSetCount1;
	int LightSetCount2;
	int LightSetCount3;
};

class GJHLight;
class GJHCamera;
class GJHRenderer;
class GJHMaskRenderer;
class GJHGameEngineTransform;
class GJHDirectRenderTarget;
class GJHDirectStructuredBuffer;
class GJHRenderManagerComponent : public GJHGameEngineComponent
{
private:
	static std::map<int, std::shared_ptr<GJHDirectRenderTarget>> m_MaskTarget;

public:
	static void CreateMaskTarget(int _MaskTargetIndex, float4 _Scale, float4 _ClearColor, DXGI_FORMAT _Fmt = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_SINT);
	static std::shared_ptr<GJHDirectRenderTarget> GetMask(int _MaskTargetIndex);

private:
	friend GJHMaskRenderer;
	friend GJHRenderer;
	friend GJHCamera;
	friend GJHLight;

private:
	std::map<int, std::list<GJHLight*>> m_LightList;
	std::map<int, std::shared_ptr<GJHDirectStructuredBuffer>> m_LightBufferList;

	std::map<int, std::list<GJHCamera*>> m_CameraList;
	std::map<int, std::list<GJHRenderer*>> m_RendererList;
	std::map<int, std::map<OutPutPathType, std::list<std::shared_ptr<GJHRenderPlayer>>>> m_RendererPlayerList;
	std::map<int, std::map<OutPutPathType, std::list<std::shared_ptr<GJHRenderPlayer>>>> m_ShadowRendererPlayerList;

	std::map<int, std::list<GJHMaskRenderer*>> m_MaskRenderList;
	LightSetData m_LightSetData;

public:
	std::map<int, std::list<GJHCamera*>>& GetCameraList()
	{
		return m_CameraList;
	}

	std::map<int, std::list<GJHLight*>>& GetLightList()
	{
		return m_LightList;
	}

private:
	void PushMaskRenderer(GJHMaskRenderer* _Render);
	void PushRenderer(GJHRenderer* _ParentRenderer);
	void PushRenderPlayer(GJHRenderer* _ParentRenderer, std::shared_ptr<GJHRenderPlayer> _RenderPlayer);
	void PushCamera(GJHCamera* _Camera);
	void PushLight(GJHLight* _Light);

	void LightUpdate(int _Order, std::vector<GJHLightData>& _LightData, GJHCamera* _Cam);

public:
	GJHRenderManagerComponent();
	~GJHRenderManagerComponent();

public:
	GJHRenderManagerComponent(const GJHRenderManagerComponent& _Other) = delete;
	GJHRenderManagerComponent(const GJHRenderManagerComponent&& _Other) = delete;

public:
	GJHRenderManagerComponent operator=(const GJHRenderManagerComponent& _Other) = delete;
	GJHRenderManagerComponent operator=(const GJHRenderManagerComponent&& _Other) = delete;

public:
	void RenderPrev() override;
	void RenderNext() override;
	void Render() override;
	void Release() override;
};