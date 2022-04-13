#include "LightData.hlsli"

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

EffectOut DefferdLightingVtx(EffectIn _In)
{
    EffectOut NewOutPut = (EffectIn) 0;
    NewOutPut.Pos = _In.Pos;
    NewOutPut.Uv = _In.Uv;
	return NewOutPut;
}

Texture2D Pos : register(t0);
Texture2D Nor : register(t1);
Texture2D Shadow : register(t2);
SamplerState Smp : register(s0);
SamplerState ShadowSmp : register(s1);

struct DefferdLightOutPut
{
    float4 Dif : SV_TARGET0;
    float4 Spc : SV_TARGET1;
    float4 Amb : SV_TARGET2;
};


DefferdLightOutPut DefferdLightingPix(EffectOut _In)
{
    DefferdLightOutPut Out = (DefferdLightOutPut)0;
    
    float4 ViewPos = Pos.Sample(Smp, _In.Uv.xy);
    float4 ViewNormal = Nor.Sample(Smp, _In.Uv.xy);
    
    if (ViewPos.w == 0)
    {
        Out.Dif = float4(0.0f, 0.0f, 0.0f, 0.0f);
        Out.Spc = float4(0.0f, 0.0f, 0.0f, 0.0f);
        Out.Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
        return Out;
    }
    
    ViewPos.w = 1.f;
    ViewNormal.w = 0.f;
    
    Out.Dif += CalDirLightDifColor(ViewNormal, LData);
    Out.Spc += CalDirLightSpcLight(ViewPos, ViewNormal, LData);
    Out.Amb += CalDirLightAmbLight(LData);

    if (0 < Out.Dif.x)
    {
        float4 vWorld = mul(float4(ViewPos.xyz, 1.0f), LData.CamInverse);
        
        float4 vShadowPos = mul(vWorld, LData.ViewProj);
        float fDepth = vShadowPos.z / vShadowPos.w;
        float UvX = vShadowPos.x / vShadowPos.w;
        float UvY = vShadowPos.y / vShadowPos.w;
        
        float2 ShadowUv = float2(UvX * 0.5f + 0.5f, UvY * -0.5f + 0.5f);
        
        if (0.001f < ShadowUv.x && ShadowUv.x < 0.999f &&
            0.001f < ShadowUv.y && ShadowUv.y < 0.999f)
        {
            float fShadowDepth = Shadow.Sample(ShadowSmp, ShadowUv.xy).r;
            
            if (0.0f < fShadowDepth && fDepth > fShadowDepth + 0.001F)
            {
                Out.Dif *= 0.01f;
                Out.Spc = 0.0f;
            }
        }
    }
    
    Out.Dif.w = 1.0f;
    Out.Spc.w = 1.0f;
    Out.Amb.w = 1.0f;

    return Out;
}