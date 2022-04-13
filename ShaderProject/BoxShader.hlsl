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

struct TransFormData
{
    float4x4 Scale;
    float4x4 Rotation;
    float4x4 Position;
    float4x4 Revolve;
    float4x4 Parent;
    float4x4 LocalWorld;
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WorldView;
    float4x4 ViewProjection;
    float4x4 WorldViewProjection;
};

cbuffer RenderingTransformData : register(b0)
{
    TransFormData Data;
}

cbuffer ColorData : register(b0)
{
    float4 Color;
}

TexVertexOut BoxVtxShader(TexVertex Vtx)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, Data.WorldViewProjection);
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

float4 BoxPixShader(TexVertexOut VertexShaderOutput) : SV_TARGET
{
    return Color;
}