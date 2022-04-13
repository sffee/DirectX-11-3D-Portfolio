cbuffer RenderingTransformData : register(b0)
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
    float4x4 ViewProjection;
    float4x4 WorldViewProjection;
    float4 CamPos;
    float4 CamPos0;
    float4 CamPos1;
    float4 CamPos2;
};
