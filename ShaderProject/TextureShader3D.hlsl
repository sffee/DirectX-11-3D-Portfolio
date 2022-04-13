#include "TransformData.hlsli"

struct VertexIn
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Uv : TEXCOORD;
};

VertexOut TextureShader3DVtx(VertexIn Vtx)
{
    VertexOut NewOutPut = (VertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
SamplerState Smp : register(s0);

float4 TextureShader3DPix(VertexOut VertexShaderOutput) : SV_TARGET
{
    float4 Color = DiffuseTexture.Sample(Smp, VertexShaderOutput.Uv.xy);
    
    return Color;
}