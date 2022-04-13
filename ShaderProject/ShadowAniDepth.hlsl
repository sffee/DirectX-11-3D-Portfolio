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

struct ShadowOut
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

ShadowOut ShadowAniDepthVtx(VertexIn _In)
{
    ShadowOut Out = (ShadowOut) 0.0f;
    Skinning(_In.Pos, _In.Weight, _In.Index, ArrAniMationMatrix);
    
    _In.Pos.w = 1.0f;
    
    Out.Pos = mul(_In.Pos, WorldViewProjection);
    Out.ProjPos = Out.Pos;
    return Out;
}

float4 ShadowAniDepthPix(ShadowOut _In) : SV_Target
{
    return float4(_In.ProjPos.z / _In.ProjPos.w, 0.0f, 0.0f, 1.0f);
}