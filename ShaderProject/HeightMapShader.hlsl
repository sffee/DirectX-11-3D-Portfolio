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

cbuffer MapData : register(b1)
{
    float4 MapCount;
    int4 IsSplatting;
};

Texture2D HTex : register(t0);
SamplerState HSmp : register(s0);

TexVertexOut HeightMapVtxShader(TexVertex Vtx)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    
    float4 HeightUv = (float4)0.f;
    
    HeightUv.x = Vtx.Uv.x / MapCount.x;
    HeightUv.z = Vtx.Uv.z / MapCount.z;
    
    float4 Hight = HTex.SampleLevel(HSmp, HeightUv.xz, 0);
    
    Vtx.Pos.y = Hight.y * MapCount.y;
    
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;

    return NewOutPut;
}

Texture2D BaseTex : register(t0);

Texture2D SDif1 : register(t1);
Texture2D SDif2 : register(t2);
Texture2D SDif3 : register(t3);
Texture2D STex1 : register(t4);
Texture2D STex2 : register(t5);
Texture2D STex3 : register(t6);

SamplerState Smp : register(s0);

float4 HeightMapPixShader(TexVertexOut VertexShaderOutput) : SV_TARGET
{
    float4 ResultColor = BaseTex.Sample(Smp, VertexShaderOutput.Uv.xz);
    
    ResultColor.w = 1.0f;
    
    if (0 != IsSplatting[1])
    {
        float4 MulColor = SDif1.Sample(Smp, VertexShaderOutput.Uv.xz);
        float2 SUv = VertexShaderOutput.Uv.xz;
        SUv.x = SUv.x / MapCount.x;
        SUv.y = SUv.y / MapCount.z;
        float4 SColor = STex1.Sample(Smp, SUv);
        ResultColor.xyz = (MulColor.xyz * SColor.x) + (ResultColor.xyz * (1.0f - SColor.x));
    }
    
    if (0 != IsSplatting[2])
    {
        float4 MulColor = SDif2.Sample(Smp, VertexShaderOutput.Uv.xz);
        float2 SUv = VertexShaderOutput.Uv.xz;
        SUv.x = SUv.x / MapCount.x;
        SUv.y = SUv.y / MapCount.z;
        float4 SColor = STex2.Sample(Smp, SUv);
        ResultColor.xyz = (MulColor.xyz * SColor.x) + (ResultColor.xyz * (1.0f - SColor.x));
    }
    
    if (0 != IsSplatting[3])
    {
        float4 MulColor = SDif3.Sample(Smp, VertexShaderOutput.Uv.xz);
        float2 SUv = VertexShaderOutput.Uv.xz;
        SUv.x = SUv.x / MapCount.x;
        SUv.y = SUv.y / MapCount.z;
        float4 SColor = STex3.Sample(Smp, SUv);
        ResultColor.xyz = (MulColor.xyz * SColor.x) + (ResultColor.xyz * (1.0f - SColor.x));
    }
    
    return ResultColor;
}
