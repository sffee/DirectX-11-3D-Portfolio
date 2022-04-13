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

MergeOut MergeVtx(MergeVertex Vtx)
{
    MergeOut NewOutPut = (MergeOut) 0;
    NewOutPut.Pos = Vtx.Pos;
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 MergePix(MergeOut _MergeOut) : SV_TARGET
{
    float4 Color = Tex.Sample(Smp, _MergeOut.Uv.xy);
    
    Color.w = saturate(Color.w);
    
    if (1.f <= Color.w)
    {
        Color.w = 1.f;
    }
    
    return Color;
}