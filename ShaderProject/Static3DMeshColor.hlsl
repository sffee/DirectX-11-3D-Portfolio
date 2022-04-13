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

VertexOut Static3DMeshColorVtx(VertexIn Vtx)
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

float4 Static3DMeshColorPix(VertexOut VertexShaderOutput) : SV_TARGET
{
    float4 DifLightLum = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 SpcLightLum = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 AmbLightLum = float4(0.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < LightSetCount.x; ++i)
    {
        DifLightLum += CalDirLightDifColor(VertexShaderOutput.ViewNormal, ArrLightData[i]);
        SpcLightLum += CalDirLightSpcLight(VertexShaderOutput.ViewPos, VertexShaderOutput.ViewNormal, ArrLightData[i]);
        AmbLightLum += CalDirLightAmbLight(ArrLightData[i]);
    }

    float4 ReturnColor;
    
    ReturnColor.xyz = Color.xyz * (DifLightLum.xyz + SpcLightLum.xyz + AmbLightLum.xyz);
    ReturnColor.w = 1.f;
    
    return ReturnColor;
}
