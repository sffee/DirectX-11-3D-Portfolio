struct LightData
{
    float4 IsOn;
    float4 DifColor;
    float4 SpcColor;
    float4 AmbColor;
    float4 Amb;
    float4 LightPower; 
    float4 SpecPow;
    float4 LightPos;
    float4 LightDir;
    float4 LightDirReverse;
    float4 CamPos; 
    float4 ShadowRange; 
    float4x4 View;
    float4x4 Proj;
    float4x4 ViewProj;
    float4x4 CamInverse;
};

StructuredBuffer<LightData> ArrLightData : register(t10);

cbuffer OneLightData : register(b11)
{
    LightData LData;
};

cbuffer LightSetData : register(b10)
{
    int4 LightSetCount;
};

float4 CalDirLightDifColor(float4 _vViewNormal, LightData _LightData)
{
    float4 DifLight = (float) 0;
    
    _vViewNormal = normalize(_vViewNormal);
    
    _LightData.LightDirReverse = normalize(_LightData.LightDirReverse);
    
    DifLight.xyz = max(0.0f, dot(_vViewNormal.xyz, _LightData.LightDirReverse.xyz));
    DifLight.w = 1.0f;
    return DifLight * _LightData.LightPower.x * _LightData.LightPower.w;
}

float4 CalDirLightAmbLight(LightData _LightData)
{
    return _LightData.Amb * _LightData.LightPower.z * _LightData.LightPower.w;
}

float4 CalDirLightSpcLight(float4 _vViewPostion, float4 _vViewNormal, LightData _LightData)
{
    float4 SpcLight = (float) 0;
    
    _vViewNormal = normalize(_vViewNormal);
    
    _LightData.LightDirReverse = normalize(_LightData.LightDirReverse);
    
    float3 vR;
    float3 vE;
    
    vR = normalize((2.0f * _vViewNormal.xyz * dot(_LightData.LightDirReverse.xyz, _vViewNormal.xyz)) - _LightData.LightDirReverse.xyz);
    vE = normalize(_LightData.CamPos.xyz - _vViewPostion.xyz);
    
    float Spc = max(0.0f, dot(vR.xyz, vE.xyz));
    
    SpcLight.xyz = pow(Spc, _LightData.SpecPow.x);
    
    SpcLight.w = 1.0f;
    
    return SpcLight * _LightData.LightPower.y * _LightData.LightPower.w;
}