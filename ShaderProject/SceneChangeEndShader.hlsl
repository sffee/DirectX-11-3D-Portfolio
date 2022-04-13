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

TexVertexOut TexVtxShader(TexVertex Vtx)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    NewOutPut.Pos = Vtx.Pos;
    NewOutPut.Uv = Vtx.Uv;
    
    return NewOutPut;
}

cbuffer DeltaTimeData : register(b0)
{
    float4 DeltaTime;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 TexPixShader(TexVertexOut VertexShaderOutput) : SV_TARGET
{
    int ScreenPosX = VertexShaderOutput.Pos.x;
    int ScreenPosY = VertexShaderOutput.Pos.y;
    
    int MaxX = (int)(1280.0f * DeltaTime.x);
    
    int TimeFrameCut = MaxX % 32.0f / 32.0f / 14.0f;
    MaxX = (MaxX / 32) * 32;
    
    int TimeFrame = MaxX / 32;
    int MyFrame = ScreenPosX / 32;
    
    float Text1Width = 1.0f / 14.0f;
    
    float4 IUv;
    IUv.x = (ScreenPosX % 32.0f) / 32.0f;
    IUv.y = (ScreenPosY % 32.0f) / 32.0f;
    
    if (TimeFrame < MyFrame)
    {
        return float4(0.f, 0.f, 0.f, 1.f);
    }
    else if (MyFrame < TimeFrame)
    {
        int PrevCut = TimeFrameCut;
        PrevCut += (TimeFrame - MyFrame);
        
        if (14 <= PrevCut + 1)
        {
            PrevCut = 14;
        }
        
        IUv.x = (IUv.x * Text1Width) + (Text1Width * PrevCut);
        IUv.y = (IUv.y * 1.0f);
        
        float4 Color = Tex.Sample(Smp, IUv.xy);
        
        if (Color.a == 0.f)
        {
            clip(-1);
        }
        else
        {
            return Tex.Sample(Smp, IUv.xy);
        }
    }
    else
    {
        IUv.x = (IUv.x * Text1Width) + (Text1Width * TimeFrameCut);
        IUv.y = (IUv.y * 1.0f);
        
        float4 Color = Tex.Sample(Smp, IUv.xy);
        
        if (Color.a == 0.f)
        {
            clip(-1);
        }
        else
        {
            return Tex.Sample(Smp, IUv.xy);
        }
    }
    
    return float4(DeltaTime.x, 0.f, 0.f, 0.f);
}