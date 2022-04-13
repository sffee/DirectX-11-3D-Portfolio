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
    float4 ViewPos : POSITION;
    float4 ViewNormal : NORMAL;
};

VertexOut DefferdFbxAniShader3DVtx(VertexIn Vtx)
{
    Skinning(Vtx.Pos, Vtx.Weight, Vtx.Index, ArrAniMationMatrix);
    Vtx.Pos.w = 1.0f;
    VertexOut NewOutPut = (VertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    
    NewOutPut.ViewPos = mul(Vtx.Pos, WorldView);
    NewOutPut.ViewPos.w = 1.0f;
    
    Vtx.Normal.w = 0.0f;
    NewOutPut.ViewNormal = normalize(mul(Vtx.Normal, WorldView));
    NewOutPut.ViewNormal.w = 0.0f;
    
    NewOutPut.Uv = Vtx.Uv;

    return NewOutPut;
}


struct DefferdOutPut
{
    float4 Dif : SV_TARGET0;
    float4 Pos : SV_TARGET1;
    float4 Nor : SV_TARGET2;
    float4 Dep : SV_TARGET3;
};

Texture2D DiffuseTexture : register(t0);
SamplerState Smp : register(s0);

DefferdOutPut DefferdFbxAniShader3DPix(VertexOut VertexShaderOutput)
{
    DefferdOutPut ReturnColor = (DefferdOutPut) 0;
    
    ReturnColor.Dif = DiffuseTexture.Sample(Smp, VertexShaderOutput.Uv.xy);
    ReturnColor.Pos = float4(VertexShaderOutput.ViewPos.xyz, 1.0f);
    ReturnColor.Nor = float4(VertexShaderOutput.ViewNormal.xyz, 1.0f);
    ReturnColor.Dep = float4(VertexShaderOutput.ViewPos.z, 0.0f, 0.0f, 1.0f);
    
    return ReturnColor;
}
