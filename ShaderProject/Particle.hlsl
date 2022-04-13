#include "TransformData.hlsli"

struct ParticleData
{
    float4 Pos;
    float4 Dir;
    float4 Scale;
    float4 Time;
};

struct VS_PARTICLE_IN
{
    float4 LocalPos : POSITION;
    float4 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct VS_PARTICLE_OUT
{
    float4 LocalPos : POSITION;
    float4 vUV : TEXCOORD;
    uint iInstID : FOG;
};

StructuredBuffer<ParticleData> ArrParticleData : register(t0);

VS_PARTICLE_OUT VS_Particle(VS_PARTICLE_IN _in)
{
    VS_PARTICLE_OUT Out = (VS_PARTICLE_OUT) 0.f;

    float4 Pos = ArrParticleData[_in.iInstID].Pos;
    Pos.w = 1.f;
    
    Out.LocalPos = mul(Pos, WorldView);

    if (0.f < ArrParticleData[_in.iInstID].Time.x)
    {
        Out.iInstID = _in.iInstID;
    }
    else
    {
        Out.iInstID = -1;
    }
    
    return Out;
}

struct GS_PARTICLE_OUT
{
    float4 LocalPos : SV_Position;
    float4 vUV : TEXCOORD;
    float iInstID : FOG;
};

[maxvertexcount(6)]
void GS_Particle(point VS_PARTICLE_OUT _in[1], inout TriangleStream<GS_PARTICLE_OUT> _Stream)
{
    if (_in[0].iInstID < 0)
    {
        return;
    }
    
    uint Index = (uint) _in[0].iInstID;
    GS_PARTICLE_OUT CreateVtx[4] =
    {
        (GS_PARTICLE_OUT) 0.f,
        (GS_PARTICLE_OUT) 0.f,
        (GS_PARTICLE_OUT) 0.f,
        (GS_PARTICLE_OUT) 0.f
    };
    
    float4 Pos = _in[0].LocalPos;
    Pos.w = 1.f;

    CreateVtx[0].LocalPos = Pos + float4(float3(-0.5f, 0.5f, 0.f), 0.f);
    CreateVtx[0].vUV = float4(0.f, 0.f, 0.f, 0.f);
    CreateVtx[0].iInstID = (float) Index;

    CreateVtx[1].LocalPos = Pos + float4(float3(0.5f, 0.5f, 0.f), 0.f);
    CreateVtx[1].vUV = float4(1.f, 0.f, 0.f, 0.f);
    CreateVtx[1].iInstID = (float) Index;
    
    CreateVtx[2].LocalPos = Pos + float4(float3(0.5f, -0.5f, 0.f), 0.f);
    CreateVtx[2].vUV = float4(1.f, 1.f, 0.f, 0.f);
    CreateVtx[2].iInstID = (float) Index;
    
    CreateVtx[3].LocalPos = Pos + float4(float3(-0.5f, -0.5f, 0.f), 0.f);
    CreateVtx[3].vUV = float4(0.f, 1.f, 0.f, 0.f);
    CreateVtx[3].iInstID = (float) Index;
    
    for (int i = 0; i < 4; ++i)
    {
        CreateVtx[i].LocalPos = mul(CreateVtx[i].LocalPos, Projection);
    }

    _Stream.Append(CreateVtx[0]);
    _Stream.Append(CreateVtx[2]);
    _Stream.Append(CreateVtx[3]);
    _Stream.RestartStrip();
    
    _Stream.Append(CreateVtx[2]);
    _Stream.Append(CreateVtx[0]);
    _Stream.Append(CreateVtx[1]);
    _Stream.RestartStrip();
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 PS_Particle(GS_PARTICLE_OUT _in) : SV_Target
{
    float4 Color = Tex.Sample(Smp, _in.vUV.xy);
    
    return Color;
}