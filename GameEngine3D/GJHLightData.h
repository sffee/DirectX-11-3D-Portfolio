#pragma once
#include <GJHGameEngineMath.h>

struct GJHLightData
{
	float4 IsOn;
	float4 DifColor;
	float4 SpcColor;
	float4 AmbColor;
	float4 Amb;
	float4 LightPower;
	float4 SpecPow;
	float4 LightPos;
	float4 LightDir;
	float4 LightDirReverse;
	float4 CamPos;
	float4 ShadowRange;
	float4x4 View;
	float4x4 Proj;
	float4x4 ViewProj;
	float4x4 CamInverse;
};