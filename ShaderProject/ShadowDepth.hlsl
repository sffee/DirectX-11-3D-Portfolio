#include "TransformData.hlsli"

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

struct ShadowOut
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

ShadowOut ShadowDepthVtx(VertexIn _In)
{
    ShadowOut Out = (ShadowOut) 0.0f;
    
    _In.Pos.w = 1.f;
    
    Out.Pos = mul(_In.Pos, WorldViewProjection);
    Out.ProjPos = Out.Pos;
    return Out;
}

float4 ShadowDepthPix(ShadowOut _In) : SV_Target
{
    return float4(_In.ProjPos.z / _In.ProjPos.w, 0.0f, 0.0f, 1.0f);
}