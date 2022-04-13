#pragma once
#include "GJHRenderer.h"

struct GRIDDATA
{
	float4 CamPos;
	float4 Color;
	float4 LineData;
};

class GJHCamera;
class GJHGridRenderer : public GJHRenderer
{
private:
	std::shared_ptr<GJHCamera> m_Target;
	GRIDDATA Data;

public:
	void SetTargetCamera(std::shared_ptr<GJHCamera> _Target);

public:
	void Start(int _Order = 0);
	void Update();

public:
	GJHGridRenderer();
	~GJHGridRenderer();

public:
	GJHGridRenderer(const GJHGridRenderer& _Other) = delete;
	GJHGridRenderer(const GJHGridRenderer&& _Other) = delete;

public:		//delete operator
	GJHGridRenderer& operator=(const GJHGridRenderer& _Other) = delete;
	GJHGridRenderer& operator=(const GJHGridRenderer&& _Other) = delete;
};

