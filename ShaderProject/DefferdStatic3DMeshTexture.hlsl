#include "TransformData.hlsli"
#include "LightData.hlsli"

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
    float4 ViewPos : POSITION;
    float4 ViewNormal : NORMAL;
    float4 ViewTangent : TANGENT;
    float4 ViewBiNormal : BINORMAL;
    float4 Uv : TEXCOORD;
};

cbuffer RenderOption : register(b11)
{
    int bBump;
    int Dummy0;
    int Dummy1;
    int Dummy2;
};

VertexOut DefferdStatic3DMeshTextureVtx(VertexIn Vtx)
{
    VertexOut NewOutPut = (VertexOut) 0;
    Vtx.Pos.w = 1.f;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    
    NewOutPut.ViewPos = mul(Vtx.Pos, WorldView);
    NewOutPut.ViewPos.w = 1.f;
    
    Vtx.Normal.w = 0.f;
    NewOutPut.ViewNormal = normalize(mul(Vtx.Normal, WorldView));
    NewOutPut.ViewNormal.w = 0.f;
    
    Vtx.BiNormal.w = 0.f;
    NewOutPut.ViewBiNormal = normalize(mul(Vtx.BiNormal, WorldView));
    NewOutPut.ViewBiNormal.w = 0.f;
    
    Vtx.Tangent.w = 0.f;
    NewOutPut.ViewTangent = normalize(mul(Vtx.Tangent, WorldView));
    NewOutPut.ViewTangent.w = 0.f;
    
    NewOutPut.Uv = Vtx.Uv;
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D BumpTexture : register(t1);
SamplerState Smp : register(s0);

struct DefferdOutPut
{
    float4 Dif : SV_TARGET0;
    float4 Pos : SV_TARGET1;
    float4 Nor : SV_TARGET2;
    float4 Dep : SV_TARGET3;
};

DefferdOutPut DefferdStatic3DMeshTexturePix(VertexOut VertexShaderOutput)
{
    DefferdOutPut ReturnColor = (DefferdOutPut)0;
    
    ReturnColor.Pos = float4(VertexShaderOutput.ViewPos.xyz, 1.f);

    if (bBump == 1)
    {
        float4 TexNormal = BumpTexture.Sample(Smp, VertexShaderOutput.Uv.xy);
    
        TexNormal = TexNormal * 2.f - 1.f;
        TexNormal.w = 0.f;
        TexNormal = normalize(TexNormal);
    
        float3x3 TangentSpace = float3x3(VertexShaderOutput.ViewTangent.xyz, VertexShaderOutput.ViewBiNormal.xyz, VertexShaderOutput.ViewNormal.xyz);
        TexNormal.xyz = mul(TexNormal.xyz, TangentSpace);
        TexNormal = normalize(TexNormal);
    
        ReturnColor.Nor = TexNormal;
        ReturnColor.Nor.w = 1.f;
    }
    else
    {
        ReturnColor.Nor = VertexShaderOutput.ViewNormal;
        ReturnColor.Nor.w = 1.f;
    }
    
    ReturnColor.Dep = float4(VertexShaderOutput.ViewPos.z, 0.f, 0.f, 1.f);
    
    ReturnColor.Dif = DiffuseTexture.Sample(Smp, VertexShaderOutput.Uv.xy);
    
    return ReturnColor;
}
