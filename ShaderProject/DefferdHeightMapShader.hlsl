#include "TransformData.hlsli"

struct TexVertex
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
    float4 Color : COLOR;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 BiNormal : BINORMAL;
};

struct TexVertexOut
{
    float4 Pos : SV_POSITION;
    float4 ViewPos : POSITION;
    float4 ViewNormal : NORMAL;
    float4 Uv : TEXCOORD;
};

cbuffer MapData : register(b1)
{
    float4 MapCount;
    int4 IsSplatting;
};

Texture2D HTex : register(t0);
SamplerState HSmp : register(s0);

TexVertexOut DefferdHeightMapVtxShader(TexVertex Vtx)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    
    float4 HeightUv = (float4)0.0f;
    HeightUv.x = Vtx.Uv.x / MapCount.x;
    HeightUv.z = Vtx.Uv.z / MapCount.z;
    
    float4 Hight = HTex.SampleLevel(HSmp, HeightUv.xz, 0);
    Vtx.Pos.y = Hight.y * MapCount.y;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;

    NewOutPut.ViewPos = mul(Vtx.Pos, WorldView);
    NewOutPut.ViewPos.w = 1.0f;
    
    Vtx.Normal.w = 0.0f;
    NewOutPut.ViewNormal = normalize(mul(Vtx.Normal, WorldView));
    NewOutPut.ViewNormal.w = 0.0f;
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


struct DefferdOutPut
{
    float4 Dif : SV_TARGET0;
    float4 Pos : SV_TARGET1;
    float4 Nor : SV_TARGET2;
    float4 Dep : SV_TARGET3;
};

DefferdOutPut DefferdHeightMapPixShader(TexVertexOut VertexShaderOutput)
{
    float4 ResultColor = BaseTex.Sample(Smp, VertexShaderOutput.Uv.xz);
    
    ResultColor.w = 1.0f;
    
    if (IsSplatting[1] != 0)
    {
        float4 MulColor = SDif1.Sample(Smp, VertexShaderOutput.Uv.xz);
        float2 SUv = VertexShaderOutput.Uv.xz;
        SUv.x = SUv.x / MapCount.x;
        SUv.y = SUv.y / MapCount.z;
        float4 SColor = STex1.Sample(Smp, SUv);
        ResultColor.xyz = (MulColor.xyz * SColor.x) + (ResultColor.xyz * (1.0f - SColor.x));
    }
    
    if (IsSplatting[2] != 0)
    {
        float4 MulColor = SDif2.Sample(Smp, VertexShaderOutput.Uv.xz);
        float2 SUv = VertexShaderOutput.Uv.xz;
        SUv.x = SUv.x / MapCount.x;
        SUv.y = SUv.y / MapCount.z;
        float4 SColor = STex2.Sample(Smp, SUv);
        ResultColor.xyz = (MulColor.xyz * SColor.x) + (ResultColor.xyz * (1.0f - SColor.x));
    }
    
    if (IsSplatting[3] != 0)
    {
        float4 MulColor = SDif3.Sample(Smp, VertexShaderOutput.Uv.xz);
        float2 SUv = VertexShaderOutput.Uv.xz;
        SUv.x = SUv.x / MapCount.x;
        SUv.y = SUv.y / MapCount.z;
        float4 SColor = STex3.Sample(Smp, SUv);
        ResultColor.xyz = (MulColor.xyz * SColor.x) + (ResultColor.xyz * (1.0f - SColor.x));
    }
    
    DefferdOutPut ReturnColor = (DefferdOutPut) 0;
    
    ReturnColor.Pos = float4(VertexShaderOutput.ViewPos.xyz, 1.0f);
    ReturnColor.Nor = float4(VertexShaderOutput.ViewNormal.xyz, 1.0f);
    ReturnColor.Dep = float4(VertexShaderOutput.ViewPos.z, 0.0f, 0.0f, 1.0f);
    ReturnColor.Dif = float4(ResultColor.xyz, 1.0f);
    
    return ReturnColor;
}
