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

MergeOut Grid2DVtx(MergeVertex Vtx)
{
    MergeOut NewOutPut = (MergeOut) 0;
    NewOutPut.Pos = Vtx.Pos;
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 Grid2DPix(MergeOut _MergeOut) : SV_TARGET
{
    int ScreenPosX = (int) _MergeOut.Pos.x;
    int ScreenPosY = (int) _MergeOut.Pos.y;

    int X = ScreenPosX % 100;
    int Y = ScreenPosY % 100;
    
    if (
        (X < 10 || 90 < X)
        ||
        (Y < 10 || 90 < Y)
       )
    {
        return float4(0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        return Tex.Sample(Smp, _MergeOut.Uv.xy);
    }
}