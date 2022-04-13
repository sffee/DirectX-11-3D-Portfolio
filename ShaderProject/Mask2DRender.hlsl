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

TexVertexOut MaskVtxShader(TexVertex Vtx)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

cbuffer MaskColor : register(b0)
{
    float4 MaskColor;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 MaskPixShader(TexVertexOut VertexShaderOutput) : SV_TARGET
{
    float4 Color = Tex.Sample(Smp, VertexShaderOutput.Uv.xy);
    
    if (Color.w != 0)
    {
        return MaskColor;
    }

    clip(-1);

}