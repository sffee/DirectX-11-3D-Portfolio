#pragma once
#include "GJHContentsComponent.h"
#include <GJHGameEngineMath.h>
#include "GJHRenderingStruct.h"
#include "GJHActorTransform.h"
#include <vector>
#include <GJHTargetPostEffect.h>

enum class CamProjectionMode
{
	Orthographic,
	Perspective
};

class GJHRenderer;
class GJHLight;
struct GJHLightData;
class LightSetData;
class GJHDefferdLighting;
class GJHDirectRenderTarget;
class GJHDirectStructuredBuffer;
class GJHCamera : public GJHContentsComponent
{

private:
	CamProjectionMode m_Mode;
	GJHActorTransform* m_Trans;
	
	float m_Fov;
	float m_Near;
	float m_Far;

	float4 m_ProjectionSize;

	std::shared_ptr<GJHDirectRenderTarget> m_CameraTarget;
	std::map<OutPutPathType, std::shared_ptr<GJHDirectRenderTarget>> m_CameraPathTarget;
	std::map<OutPutPathType, std::shared_ptr<GJHDirectRenderTarget>> m_LightTarget;
	std::shared_ptr<GJHDefferdLighting> m_DefferdTargetPostEffect;

public:
	std::shared_ptr<GJHDirectStructuredBuffer> m_pLightStructuredBuffer;
	GJHLight* CurDefferdLight;
	GJHLightData* OneLightData;
	LightSetData* m_pLightSetData;

private:
	std::vector<int> m_ViewList;
	std::vector<OutPutPathType> m_PathList;

public:
	float GetFar()
	{
		return m_Far;
	}

	std::shared_ptr<GJHDirectRenderTarget> GetLightTarget(OutPutPathType _Type)
	{
		if (m_LightTarget.find(_Type) == m_LightTarget.end())
		{
			return nullptr;
		}

		return m_LightTarget[_Type];
	}

	std::shared_ptr<GJHDirectRenderTarget> GetPathTarget(OutPutPathType _Type)
	{
		if (m_CameraPathTarget.find(_Type) == m_CameraPathTarget.end())
		{
			return nullptr;
		}

		return m_CameraPathTarget[_Type];
	}

	std::shared_ptr<GJHDirectRenderTarget> GetCameraTarget()
	{
		return m_CameraTarget;
	}

	float4 GetViewCamPos();
	float4x4 GetViewMatrix();

public:
	void CamTargetMerge();
	void CamTargetClear();
	void CamTargetSetting();
	bool CamPathTargetSetting(OutPutPathType _Path);

public:
	int m_Order;
	bool m_ChangeOrder;

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
	void SetCamProjectionSize(float4 _Size)
	{
		m_ProjectionSize = _Size;
	}

	float4 GetCamProjectionSize()
	{
		return m_ProjectionSize;
	}

	void SetCamProjectionMode(CamProjectionMode _Mode)
	{
		m_Mode = _Mode;
	}

	inline GJHRenderingTransformData& GetCameraTransformMatrix()
	{
		return m_Trans->GetRenderingData();
	}

public:
	GJHCamera();
	~GJHCamera();

public:
	GJHCamera(const GJHCamera& _Other) = delete;
	GJHCamera(const GJHCamera&& _Other) = delete;

public:
	GJHCamera operator=(const GJHCamera& _Other) = delete;
	GJHCamera operator=(const GJHCamera&& _Other) = delete;

public:
	std::vector<OutPutPathType>& GetPathList()
	{
		return m_PathList;
	}

	std::vector<int>& GetViewList()
	{
		return m_ViewList;
	}

public:
	void InitCam(int _Order);

public:
	template<typename ... REST>
	void Start(int _Order, REST ... _Arg)
	{
		PushList(_Arg...);
		InitCam(_Order);
	}

	template<typename ... REST >
	void PushList(OutPutPathType _ViewOrder, REST ... _Arg)
	{
		m_PathList.push_back(_ViewOrder);
		PushList(_Arg...);
	}

	template<typename ... REST>
	void PushList(int _ViewOrder, REST ... _Arg)
	{
		m_ViewList.push_back(_ViewOrder);
		PushList(_Arg...);
	}

	void PushList()
	{

	}

public:
	void DefferdLightTargetClear();
	void DefferdLightTargetProcess(OutPutPathType _OutPutType);

	void RenderPrev() override;
	void CalCameraMatrix();

private:
	void PathTargetCreate(OutPutPathType Type);
};