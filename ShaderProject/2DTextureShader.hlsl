#include "TransformData.hlsli"
    
struct TexVertex
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
};

struct TexVertexOut
{
    float4 Pos : SV_POSITION;
    float4 Uv : TEXCOORD;
};

cbuffer SpriteCutData : register(b1)
{
    float2 StartPos;
    float2 Size;
}

TexVertexOut TexVtxShader(TexVertex Vtx, uint IndexBuffer : SV_VertexID)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv.x = (Vtx.Uv.x * Size.x) + StartPos.x;
    NewOutPut.Uv.y = (Vtx.Uv.y * Size.y) + StartPos.y;
    
    return NewOutPut;
}

cbuffer LastEffectColor : register(b0)
{
    float4 EffectColor;
}

cbuffer LastOnlyColor : register(b1)
{
    float4 OnlyColor;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 TexPixShader(TexVertexOut VertexShaderOutput) : SV_TARGET
{
    float4 Color = Tex.Sample(Smp, VertexShaderOutput.Uv.xy);
    
    if (OnlyColor.x == 1.f
        && Color.a != 0.f)
    {
        return EffectColor;
    }
    else
    {
        return Color * EffectColor;
    }
}