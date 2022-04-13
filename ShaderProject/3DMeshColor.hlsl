#include "TransformData.hlsli"

struct VertexIn
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Uv : TEXCOORD;
};

VertexOut MeshColorVtx(VertexIn Vtx)
{
    VertexOut NewOutPut = (VertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

cbuffer OutPutColor : register(b1)
{
    float4 Color;
};

float4 MeshColorPix(VertexOut VertexShaderOutput) : SV_TARGET
{
    return Color;
}
