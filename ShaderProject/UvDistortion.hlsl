struct EffectIn
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
};

struct EffectOut
{
    float4 Pos : SV_POSITION;
    float4 Uv : TEXCOORD;
};

EffectOut UvdistortionVtx(EffectIn _In)
{
    EffectOut NewOutPut = (EffectIn) 0;
    NewOutPut.Pos = _In.Pos;
    NewOutPut.Uv = _In.Uv;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

cbuffer UvdistortionData : register(b0)
{
    float4 TimeValue;
};

float4 UvdistortionPix(EffectOut _In) : SV_TARGET
{
    float2 uv = _In.Uv;
    float MinPower = TimeValue.x;
    float Power = _In.Uv.x + TimeValue.x;
    
    uv.x += sin(Power * 10) / (100 + MinPower * 100);
    uv.y += cos(Power * 10) / 50;
    
    return Tex.Sample(Smp, uv.xy);
}