struct MergeVertex
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
};

struct MergeOut
{
    float4 Pos : SV_POSITION;
    float4 Uv : TEXCOORD;
};

MergeOut OutLineVtx(MergeVertex Vtx)
{
    MergeOut NewOutPut = (MergeOut) 0;
    NewOutPut.Pos = Vtx.Pos;
    NewOutPut.Uv = Vtx.Uv;
    return NewOutPut;
}

cbuffer OutLineData : register(b0)
{
    float4 LineColor;
    float4 Option;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 OutLinePix(MergeOut _MergeOut) : SV_TARGET
{
    float2 OnPixelUVX = float2(1.0f / 800.0f, 0.0f);
    float2 OnPixelUVY = float2(0.0f, 1.0f / 800.0f);
    
    float4 Color = Tex.Sample(Smp, _MergeOut.Uv.xy);
    
    float2 LUv = _MergeOut.Uv.xy - OnPixelUVX * Option.x;
    float2 RUv = _MergeOut.Uv.xy + OnPixelUVX * Option.x;
    float2 TUv = _MergeOut.Uv.xy + OnPixelUVY * Option.x;
    float2 BUv = _MergeOut.Uv.xy - OnPixelUVY * Option.x;
    
    float2 LTUv = LUv + (TUv - LUv) * 0.5f;
    float2 LBUv = LUv + (BUv - LUv) * 0.5f;
    float2 RTUv = RUv + (TUv - RUv) * 0.5f;
    float2 RBUv = RUv + (BUv - RUv) * 0.5f;
    
    float4 LColor = Tex.Sample(Smp, LUv);
    float4 RColor = Tex.Sample(Smp, RUv);
    float4 TColor = Tex.Sample(Smp, TUv);
    float4 BColor = Tex.Sample(Smp, BUv);
    float4 LTColor = Tex.Sample(Smp, LTUv);
    float4 LBColor = Tex.Sample(Smp, LBUv);
    float4 RTColor = Tex.Sample(Smp, RTUv);
    float4 RBColor = Tex.Sample(Smp, RBUv);
    
    if (Color.w == 0.0f &&
        (LColor.w != 0.0f ||
        RColor.w != 0.0f ||
        TColor.w != 0.0f ||
        BColor.w != 0.0f ||
        LTColor.w != 0.0f ||
        LBColor.w != 0.0f ||
        RTColor.w != 0.0f ||
        RBColor.w != 0.0f)
       )
    {
        return LineColor;
    }
    
    return Tex.Sample(Smp, _MergeOut.Uv.xy);
}