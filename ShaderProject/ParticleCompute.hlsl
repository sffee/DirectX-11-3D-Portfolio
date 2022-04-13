

struct ParticleData
{
    float4 Pos;
    float4 Dir;
    float4 Scale;
    float4 Time;
};


RWStructuredBuffer<ParticleData> g_particleBuffer : register(u0);
RWStructuredBuffer<int4> g_particleLive : register(u1);

[numthreads(1024, 1, 1)]
void ParticleCompute(int3 _ThreadIdx : SV_DispatchThreadID)
{
    g_particleBuffer[_ThreadIdx.x].Pos.x = 999.0f;
    g_particleBuffer[_ThreadIdx.x].Pos.y = 9999.0f;
    g_particleBuffer[_ThreadIdx.x].Pos.z = 99999.0f;
    return;
}