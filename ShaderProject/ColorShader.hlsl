#include "TransformData.hlsli"

struct ColorVertex
{
    float4 Pos : POSITION;
};

struct ColorVertexOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

ColorVertexOut VtxColorShader(ColorVertex Vtx)
{
    ColorVertexOut NewOutPut = (ColorVertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, WorldViewProjection);
    
    return NewOutPut;
}

float4 PixColorShader(ColorVertexOut VertexShaderOutput)	: SV_TARGET
{
    return float4(0.f, 1.f, 0.f, 1.f);

}