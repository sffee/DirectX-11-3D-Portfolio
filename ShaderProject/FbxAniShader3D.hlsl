#include "TransformData.hlsli"
#include "AniHeader.hlsli"

struct VertexIn
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
    float4 Weight : BLENDWEIGHT;
    int4 Index : BLENDINDICES;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Uv : TEXCOORD;
};

VertexOut FbxAniShader3DVtx(VertexIn Vtx)
{
    Skinning(Vtx.Pos, Vtx.Weight, Vtx.Index, ArrAniMationMatrix);
    Vtx.Pos.w = 1.0f;
    
    VertexOut NewOutPut = (VertexOut) 0;
    
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection); // 0.5f
    NewOutPut.Uv = Vtx.Uv;

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
SamplerState Smp : register(s0);

float4 FbxAniShader3DPix(VertexOut VertexShaderOutput) : SV_TARGET
{
    float4 Color = DiffuseTexture.Sample(Smp, VertexShaderOutput.Uv.xy);
  
    return Color;
}
