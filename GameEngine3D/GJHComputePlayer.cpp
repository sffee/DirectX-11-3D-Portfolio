#include "GJHComputePlayer.h"

GJHComputePlayer::GJHComputePlayer()
{
}

GJHComputePlayer::~GJHComputePlayer()
{
}

void GJHComputePlayer::SetComputeShader(const GJHGameEngineString& _Name, int _X, int _Y, int _Z)
{
    m_ComputeShader = GJHDirectComputeShader::Find(_Name);

    ShaderSettingDataAutoCreate(m_ComputeShader.get());

    SetComputeThreadSetting(_X, _Y, _Z);
}

void GJHComputePlayer::SetComputeThreadSetting(int _X, int _Y, int _Z)
{
    ThreadX = _X;
    ThreadY = _Y;
    ThreadZ = _Z;
}

void GJHComputePlayer::Compute()
{
    if (m_ComputeShader == nullptr)
    {
        GJHGameEngineDebug::AssertMsg("if (nullptr == m_ComputeShader)");
    }

    AllResSetting();

    m_ComputeShader->Setting();
    m_ComputeShader->Compute(ThreadX, ThreadY, ThreadZ);
}