struct AniMat
{
    float4x4 Mat;
};

void Skinning(inout float4 _Pos, float4 _Weight, int4 _Index, StructuredBuffer<AniMat> _ArrAniMationMatrix)
{
    float4 CalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _Pos.w = 1.0F;

    _Weight[3] = 1.f - _Weight[0] - _Weight[1] - _Weight[2];

    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrAniMationMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Pos, Mat.Mat);
    }
    
    _Pos = CalPos;
}

StructuredBuffer<AniMat> ArrAniMationMatrix : register(t0);