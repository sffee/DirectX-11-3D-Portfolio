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

EffectOut DefferdMergeVtx(EffectIn _In)
{
    EffectOut NewOutPut = (EffectIn) 0;
    NewOutPut.Pos = _In.Pos;
    NewOutPut.Uv = _In.Uv;
	return NewOutPut;
}

Texture2D Dif : register(t0);
Texture2D DifLight : register(t1);
Texture2D SpcLight : register(t2);
Texture2D AmbLight : register(t3);
SamplerState Smp : register(s0);


float4 DefferdMergePix(EffectOut _In) : SV_Target
{
    float4 DifColor = Dif.Sample(Smp, _In.Uv.xy);
    float4 DifLightColor = DifLight.Sample(Smp, _In.Uv.xy);
    float4 SpcLightColor = SpcLight.Sample(Smp, _In.Uv.xy);
    float4 AmbLightColor = AmbLight.Sample(Smp, _In.Uv.xy);
    
    if (0.0f >= DifColor.x &&
        0.0f >= DifColor.y &&
        0.0f >= DifColor.z &&
        0.0f >= DifColor.w )
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    float4 ReturnColor;
    ReturnColor.xyz = DifColor.xyz * (DifLightColor.xyz + SpcLightColor.xyz + AmbLightColor.xyz);
    ReturnColor.w = 1.0f;
    
    return ReturnColor;
}