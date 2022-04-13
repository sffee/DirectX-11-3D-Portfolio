

struct InVtx
{
    // �̳༮�� ��� 16
    //     1    1    1    1
    // floatfloatfloatfloat
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
};

struct OutVtx
{
    // �����Ͷ������� �� �������� �������� �ȼ��� ������ ���ϴ�.
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
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
    float4x4 ViewPorjection;
    float4x4 WorldViewPorjection;
};

cbuffer RenderingTransformData : register(b0)
{
    TransFormData Data;
};


OutVtx EngineGrid3DVtx(InVtx Vtx)
{
    OutVtx NewOutPut = (OutVtx) 0;
    NewOutPut.Pos = mul(Vtx.Pos, Data.WorldViewPorjection); // 0.5f
    NewOutPut.WorldPos = mul(Vtx.Pos, Data.World); // 0.5f
    NewOutPut.Uv = Vtx.Uv;
	return NewOutPut;
}

cbuffer GRIDDATA : register(b0)
{
    float4 CamPos;
    float4 Color;
    float4 LineData;
};

float4 EngineGrid3DPix(OutVtx _VtxOut) : SV_TARGET
{
    int ScreenPosX = (int) _VtxOut.WorldPos.x;
    int ScreenPosZ = (int) _VtxOut.WorldPos.z;
    
    // ���̴����� ������ �����ڰ� �Ƴ�?
    // 0~100������ �ȼ�����?
    // 0~10���� ����� �ϰ� �ʹ�....
    
    float BaseRatio = 100.0f;
    float Level = 1.0f;
    float LineWidth = LineData.x;
    float CurRatio = BaseRatio * LineData.y;
    
    int X = abs(ScreenPosX) % CurRatio;
    int Z = abs(ScreenPosZ) % CurRatio;
    
    if (
        (X < LineWidth || X > CurRatio - LineWidth)
        ||
        (Z < LineWidth || Z > CurRatio - LineWidth)
        )
    {
        return Color;
    }
    else
    {
        clip(-1);
        // return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
     
}