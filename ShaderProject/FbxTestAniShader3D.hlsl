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

struct AniMat
{
    float4x4 Mat;
};

void Skinning(inout float4 _Pos, float4 _Weight, int4 _Index, StructuredBuffer<AniMat> _ArrAniMationMatrix)
{
    float4 CalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _Pos.w = 1.0F;

    _Weight[3] = 1.f - _Weight[0] - _Weight[1] - _Weight[2];

    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrAniMationMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Pos, Mat.Mat);
    }
    _Pos = CalPos;
}

StructuredBuffer<AniMat> ArrAniMationMatrix : register(t0);

VertexOut FbxTestAniShader3DVtx(VertexIn Vtx)
{
    Skinning(Vtx.Pos, Vtx.Weight, Vtx.Index, ArrAniMationMatrix);
    Vtx.Pos.w = 1.0f;
    VertexOut NewOutPut = (VertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;

    return NewOutPut;
}

Texture2D Texture : register(t0);
SamplerState Smp : register(s0);

float4 FbxTestAniShader3DPix(VertexOut VertexShaderOutput) : SV_TARGET
{
    float4 Color = Texture.Sample(Smp, VertexShaderOutput.Uv.xy);
    
    return Color;
}
