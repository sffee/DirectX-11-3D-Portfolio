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
    float4 Uv : TEXCOORD;
};

VertexOut DefferdStatic3DMeshColorVtx(VertexIn Vtx)
{
    VertexOut NewOutPut = (VertexOut) 0;
    Vtx.Pos.w = 1.0f;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.ViewPos = mul(Vtx.Pos, WorldView);
    NewOutPut.ViewPos.w = 1.0f;
    
    Vtx.Normal.w = 0.0f;
    NewOutPut.ViewNormal = normalize(mul(Vtx.Normal, WorldView));
    NewOutPut.ViewNormal.w = 0.0f;
    NewOutPut.Uv = Vtx.Uv;
    return NewOutPut;
}

cbuffer OutPutColor : register(b1)
{
    float4 Color;
};

struct DefferdOutPut
{
    float4 Dif : SV_TARGET0;
    float4 Pos : SV_TARGET1;
    float4 Nor : SV_TARGET2;
    float4 Dep : SV_TARGET3;
};

DefferdOutPut DefferdStatic3DMeshColorPix(VertexOut VertexShaderOutput)
{
    DefferdOutPut ReturnColor = (DefferdOutPut)0;
    
    ReturnColor.Pos = float4(VertexShaderOutput.ViewPos.xyz, 1.0f);
    ReturnColor.Nor = float4(VertexShaderOutput.ViewNormal.xyz, 1.0f);
    ReturnColor.Dep = float4(VertexShaderOutput.ViewPos.z, 0.0f, 0.0f, 1.0f);
    ReturnColor.Dif = Color;
    
    return ReturnColor;
}
