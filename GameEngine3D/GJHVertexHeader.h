#pragma once
#include <GJHGameEngineMath.h>

struct VertexBase
{
public:
	float4 Pos;
	float4 Uv;
	float4 Color;
	float4 Normal;
	float4 Tangent;
	float4 BiNormal;
};

struct ParticleVTX
{
public:
	float4 Pos;
	float4 Uv;
	int Index;
};

struct VertexAnimation
{
public:
	float4 Pos;
	float4 Uv;
	float4 Color;
	float4 Normal;
	float4 BiNormal;
	float4 Tangent;
	float4 Weight;
	int Index[4];
};