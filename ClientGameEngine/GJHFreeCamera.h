#pragma once
#include "GJHContentsComponent.h"

class GJHFreeCameraWindow;
class GJHFreeCamera : public GJHContentsComponent
{
private:
	friend GJHFreeCameraWindow;

private:
	float m_BoostRatio;
	float m_Speed;
	float m_RotSpeed;

	bool m_FreeCamMode;

public:
	void FreeCamModeOn()
	{
		m_FreeCamMode = true;
	}

	void FreeCamModeOff()
	{
		m_FreeCamMode = false;
	}

public:
	GJHFreeCamera();
	~GJHFreeCamera();

public:
	GJHFreeCamera(const GJHFreeCamera& _Other) = delete;
	GJHFreeCamera(const GJHFreeCamera&& _Other) = delete;

public:
	GJHFreeCamera& operator=(const GJHFreeCamera& _Other) = delete;
	GJHFreeCamera& operator=(const GJHFreeCamera&& _Other) = delete;

public:
	void Start() override;
	void Update() override;
};

