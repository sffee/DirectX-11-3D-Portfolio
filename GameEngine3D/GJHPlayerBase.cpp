#include "GJHPlayerBase.h"
#include "GJHDirectSampler.h"
#include "GJHDirectVertexShader.h"
#include "GJHDirectPixelShader.h"
#include "GJHDirectRenderTarget.h"

GJHPlayerBase::GJHPlayerBase()
{
}

GJHPlayerBase::~GJHPlayerBase()
{
}

void GJHPlayerBase::ShaderSettingDataAutoCreate(GJHDirectShader* _Shader)
{
    if (_Shader == nullptr)
    {
        return;
    }

    for (auto& _Data : _Shader->m_SettingDataMap)
    {
        ShaderData& SettingData = _Data.second;

        D3D_SHADER_INPUT_TYPE ResType = SettingData.m_ResType;
        switch (ResType)
        {
        case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER:
            CreateCBufferSetting(SettingData.Name, SettingData.m_ShaderType, SettingData.m_Reg, SettingData.m_Size);
            break;
        case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE:
            CreateTextureSetting(SettingData.Name, SettingData.m_ShaderType, SettingData.m_Reg);
            break;
        case D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER:
            CreateSamplerSetting(SettingData.Name, SettingData.m_ShaderType, SettingData.m_Reg);
            break;
        case D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED:
            CreateSBufferSetting(SettingData.Name, SettingData.m_ShaderType, SettingData.m_Reg);
            break;
        case D3D_SHADER_INPUT_TYPE::D3D11_SIT_UAV_RWSTRUCTURED:
            CreateRBufferSetting(SettingData.Name, SettingData.m_ShaderType, SettingData.m_Reg);
            break;
        default:
            GJHGameEngineDebug::AssertMsg("D3D_SHADER_INPUT_TYPE Default");
            break;
        }
    }
}

void GJHPlayerBase::AllTextureSetting()
{
    for (int i = 0; i < m_VecTextureSetter.size(); ++i)
    {
        m_VecTextureSetter[i]->Setting();
    }
}

void GJHPlayerBase::AllTextureReset()
{
    for (int i = 0; i < m_VecTextureSetter.size(); ++i)
    {
        m_VecTextureSetter[i]->Reset();
    }
}

void GJHPlayerBase::AllSamplerSetting()
{
    for (int i = 0; i < m_VecSamplerSetter.size(); ++i)
    {
        m_VecSamplerSetter[i]->Setting();
    }
}

void GJHPlayerBase::AllCBufferSetting()
{
    for (int i = 0; i < m_VecCBufferSetter.size(); ++i)
    {
        m_VecCBufferSetter[i]->Setting();
    }
}

void GJHPlayerBase::AllSBufferSetting()
{
    for (int i = 0; i < m_VecSBufferSetter.size(); ++i)
    {
        m_VecSBufferSetter[i]->Setting();
    }
}


void GJHPlayerBase::AllRBufferSetting()
{
    for (int i = 0; i < m_VecRBufferSetter.size(); ++i)
    {
        m_VecRBufferSetter[i]->Setting();
    }
}

void GJHPlayerBase::AllResSetting()
{
    AllTextureSetting();
    AllSamplerSetting();
    AllCBufferSetting();
    AllSBufferSetting();
    AllRBufferSetting();
}


void GJHPlayerBase::CreateTextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg)
{
    if (m_TextureSetter[_Name].end() != m_TextureSetter[_Name].find(_ShaderType))
    {
        return;
    }

    TextureSetter& Setter = m_TextureSetter[_Name][_ShaderType];
    Setter.Name = _Name;
    Setter.m_Reg = _Reg;
    Setter.m_Type = _ShaderType;
    Setter.Init(GJHDirectTexture2D::Find(L"EngineErrorTex.png"));
    m_VecTextureSetter.push_back(&Setter);
}

void GJHPlayerBase::CreateSamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg)
{
    if (m_SamplerSetter[_Name].end() != m_SamplerSetter[_Name].find(_ShaderType))
    {
        return;
    }

    SamplerSetter& Setter = m_SamplerSetter[_Name][_ShaderType];
    Setter.Name = _Name;
    Setter.m_Reg = _Reg;
    Setter.m_Type = _ShaderType;
    Setter.Init(GJHDirectSampler::Find(L"SMPLINEAR"));
    m_VecSamplerSetter.push_back(&Setter);
}
void GJHPlayerBase::CreateCBufferSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg, size_t _Size)
{
    if (m_CBufferSetter[_Name].end() != m_CBufferSetter[_Name].find(_ShaderType))
    {
        return;
    }

    CBufferSetter& Setter = m_CBufferSetter[_Name][_ShaderType];
    Setter.Name = _Name;
    Setter.m_Reg = _Reg;
    Setter.m_Type = _ShaderType;

    Setter.CBuffer = std::make_shared<GJHDirectConstantBuffer>();
    Setter.CBuffer->Create(_Size, _ShaderType, _Reg);
    Setter.Init();

    m_VecCBufferSetter.push_back(&Setter);
}

void GJHPlayerBase::CreateSBufferSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg)
{
    if (m_SBufferSetter[_Name].end() != m_SBufferSetter[_Name].find(_ShaderType))
    {
        return;
    }

    SBufferSetter& Setter = m_SBufferSetter[_Name][_ShaderType];
    Setter.Name = _Name;
    Setter.m_Reg = _Reg;
    Setter.m_Type = _ShaderType;

    Setter.SBuffer = std::make_shared<GJHDirectStructuredBuffer>();
    Setter.Init();

    m_VecSBufferSetter.push_back(&Setter);
}

void GJHPlayerBase::CreateRBufferSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _Reg)
{
    if (m_RBufferSetter[_Name].end() != m_RBufferSetter[_Name].find(_ShaderType))
    {
        return;
    }

    RBufferSetter& Setter = m_RBufferSetter[_Name][_ShaderType];
    Setter.Name = _Name;
    Setter.m_Reg = _Reg;
    Setter.m_Type = _ShaderType;

    Setter.RBuffer = std::make_shared<GJHDirectStructuredBuffer>();
    Setter.Init();

    m_VecRBufferSetter.push_back(&Setter);
}

bool GJHPlayerBase::IsTextureSetting(const GJHGameEngineString& _Name)
{
    if (m_TextureSetter.end() != m_TextureSetter.find(_Name))
    {
        return true;
    }

    return false;
}
bool GJHPlayerBase::IsSamplerSetting(const GJHGameEngineString& _Name)
{
    if (m_SamplerSetter.end() != m_SamplerSetter.find(_Name))
    {
        return true;
    }

    return false;
}
bool GJHPlayerBase::IsCBufferSetting(const GJHGameEngineString& _Name)
{
    if (m_CBufferSetter.end() != m_CBufferSetter.find(_Name))
    {
        return true;
    }

    return false;
}

bool GJHPlayerBase::IsSBufferSetting(const GJHGameEngineString& _Name)
{
    if (m_SBufferSetter.end() != m_SBufferSetter.find(_Name))
    {
        return true;
    }

    return false;
}

bool GJHPlayerBase::IsRBufferSetting(const GJHGameEngineString& _Name)
{
    if (m_RBufferSetter.end() != m_RBufferSetter.find(_Name))
    {
        return true;
    }

    return false;
}

void GJHPlayerBase::TextureSetting(const GJHGameEngineString& _Name, std::shared_ptr<GJHDirectTexture2D> _Res) {

    if (m_TextureSetter.end() == m_TextureSetter.find(_Name))
    {
        GJHGameEngineDebug::AssertMsg("그런 이름의 텍스처를 사용한적이 없습니다. 혹은 존재하지 않는 텍스처 세팅입니다.");
    }

    for (auto& Setter : m_TextureSetter[_Name])
    {
        TextureSetting(_Name, Setter.first, _Res);
    }

}

void GJHPlayerBase::TextureSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _TexName)
{
    if (m_TextureSetter.end() == m_TextureSetter.find(_Name))
    {
        GJHGameEngineDebug::AssertMsg("그런 이름의 텍스처를 사용한적이 없습니다. 혹은 존재하지 않는 텍스처 세팅입니다.");
    }

    for (auto& Setter : m_TextureSetter[_Name])
    {
        TextureSetting(_Name, Setter.first, GJHDirectTexture2D::Find(_TexName));
    }
}

void GJHPlayerBase::TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName)
{
    if (m_TextureSetter.end() == m_TextureSetter.find(_Name))
    {
        GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
    }


    TextureSetting(_Name, _ShaderType, GJHDirectTexture2D::Find(_TexName));
}

void GJHPlayerBase::TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectTexture2D> _Texture)
{
    if (nullptr == _Texture)
    {
        GJHGameEngineDebug::AssertMsg("if (nullptr == _Texture)");
    }

    m_TextureSetter[_Name][_ShaderType].Init(_Texture);
}


void GJHPlayerBase::SamplerSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _SmpName)
{
    if (m_SamplerSetter.end() == m_SamplerSetter.find(_Name))
    {
        GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
    }

    for (auto& Setter : m_SamplerSetter[_Name])
    {
        SamplerSetting(_Name, Setter.first, GJHDirectSampler::Find(_SmpName));
    }

}

void GJHPlayerBase::SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName)
{
    if (m_SamplerSetter.end() != m_SamplerSetter.find(_Name))
    {
        GJHGameEngineDebug::AssertMsg("if (m_TextureSetter.end() != m_TextureSetter.find(_Name))");
    }

    SamplerSetting(_Name, _ShaderType, GJHDirectSampler::Find(_TexName));
}

void GJHPlayerBase::SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectSampler> _Res)
{
    if (nullptr == _Res)
    {
        GJHGameEngineDebug::AssertMsg("if (nullptr == _Res)");
    }

    m_SamplerSetter[_Name][_ShaderType].Init(_Res);

}

std::vector<std::shared_ptr<GJHDirectTexture2D>> GJHPlayerBase::GetTexture(const GJHGameEngineString& _Name)
{
    std::vector<std::shared_ptr<GJHDirectTexture2D>> TexVec;

    for (auto& Setter : m_TextureSetter[_Name])
    {
        std::shared_ptr<GJHDirectTexture2D> Tex = GetTexture(_Name, Setter.first);

        if (nullptr != Tex)
        {
            TexVec.push_back(Tex);
        }
    }

    return TexVec;
}

std::shared_ptr<GJHDirectTexture2D> GJHPlayerBase::GetTexture(const GJHGameEngineString& _Name, ShaderType _ShaderType)
{
    return m_TextureSetter[_Name][_ShaderType].m_Texture;
}