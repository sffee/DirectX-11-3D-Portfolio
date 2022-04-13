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

MergeOut MaskCullingVtx(MergeVertex Vtx)
{
    MergeOut NewOutPut = (MergeOut) 0;
    NewOutPut.Pos = Vtx.Pos;
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
Texture2D MaskTex : register(t1);
SamplerState Smp : register(s0);

float4 MaskCullingPix(MergeOut _MergeOut) : SV_TARGET
{
    float4 Color = MaskTex.Sample(Smp, _MergeOut.Uv.xy);
    
    if (Color.r == 0)
    {
        clip(-1);
    }

    return Tex.Sample(Smp, _MergeOut.Uv.xy);

}