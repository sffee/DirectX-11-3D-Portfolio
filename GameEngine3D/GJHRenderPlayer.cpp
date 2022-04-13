#include "GJHRenderPlayer.h"
#include "GJHDirectSampler.h"
#include "GJHDirectVertexShader.h"
#include "GJHDirectPixelShader.h"
#include "GJHDirectRenderTarget.h"

GJHRenderPlayer::GJHRenderPlayer() :
    m_RenderType(RENDERINGTYPE::NORMAL),
    m_InstanceCount(0)
{
}

GJHRenderPlayer::~GJHRenderPlayer()
{
}

void GJHRenderPlayer::SetVertexShader(const GJHGameEngineString& _Name)
{
    if (true == m_Material->m_IsOriginal)
    {
        m_Material = m_Material->Clone();
    }

    m_Material->SetVertexShader(_Name);
    ReLoadingSettingData();
}

void GJHRenderPlayer::SetGeometryShader(const GJHGameEngineString& _Name)
{

    if (true == m_Material->m_IsOriginal)
    {
        m_Material = m_Material->Clone();
    }

    m_Material->SetGeometryShader(_Name);
    ReLoadingSettingData();

}

void GJHRenderPlayer::SetPixelShader(const GJHGameEngineString& _Name)
{
    if (true == m_Material->m_IsOriginal)
    {
        m_Material = m_Material->Clone();
    }

    m_Material->SetPixelShader(_Name);
    ReLoadingSettingData();
}

void GJHRenderPlayer::SetRasterizer(const GJHGameEngineString& _Name)
{
    if (true == m_Material->m_IsOriginal)
    {
        m_Material = m_Material->Clone();
    }

    m_Material->SetRasterizer(_Name);
}
void GJHRenderPlayer::SetDepthStencil(const GJHGameEngineString& _Name)
{
    if (true == m_Material->m_IsOriginal)
    {
        m_Material = m_Material->Clone();
    }

    m_Material->SetDepthStencil(_Name);
}
void GJHRenderPlayer::SetBlend(const GJHGameEngineString& _Name)
{
    if (true == m_Material->m_IsOriginal)
    {
        m_Material = m_Material->Clone();
    }

    m_Material->SetBlend(_Name);
}

void GJHRenderPlayer::ReLoadingSettingData()
{
    SetMaterial(m_Material);
}

void GJHRenderPlayer::SetMaterial(const GJHGameEngineString& _Name, std::function<void()> _ResultPtr)
{
    m_Material = GJHMaterial::Find(_Name);
    SetMaterial(m_Material, _ResultPtr);
}

void GJHRenderPlayer::SetMaterial(std::shared_ptr<GJHMaterial> _Mat, std::function<void()> _ResultPtr)
{
    SettingDataClear();

    if (nullptr == m_Material)
    {
        GJHGameEngineDebug::AssertMsg("if (nullptr == m_Material)");
    }

    {
        GJHDirectShader* Vertex = reinterpret_cast<GJHDirectShader*>(m_Material->GetVertexShader().get());
        ShaderSettingDataAutoCreate(Vertex);
    }


    {
        GJHDirectShader* Geometry = reinterpret_cast<GJHDirectShader*>(m_Material->GetGeometryShader().get());
        ShaderSettingDataAutoCreate(Geometry);
    }


    {
        GJHDirectShader* Pixel = reinterpret_cast<GJHDirectShader*>(m_Material->GetPixelShader().get());
        ShaderSettingDataAutoCreate(Pixel);
    }

    if (_ResultPtr != nullptr)
    {
        _ResultPtr();
    }
}

void GJHRenderPlayer::SetMesh(const GJHGameEngineString& _Name)
{
    SetMesh(GJHDirectMesh::Find(_Name));
}

void GJHRenderPlayer::SetMesh(std::shared_ptr<GJHDirectMesh> _Mesh)
{
    m_Mesh = _Mesh;
    if (m_Mesh == nullptr)
    {
        GJHGameEngineDebug::AssertMsg("if (nullptr == m_Mesh)");
    }
}

void GJHRenderPlayer::Render(std::shared_ptr<GJHDirectRenderTarget> _Target)
{
    _Target->Setting();

    Render();
}

void GJHRenderPlayer::Render()
{
    m_Material->Setting();
    AllResSetting();

    m_Mesh->Setting();
    
    switch (m_RenderType)
    {
    case RENDERINGTYPE::NORMAL:
        m_Mesh->RenderIndexed();
        break;
    case RENDERINGTYPE::INSTANCE:
        if (0 < m_InstanceCount)
        {
            m_Mesh->RenderIndexedInstanced(m_InstanceCount);
        }
        break;
    default:
        break;
    }
}

OutPutPathType GJHRenderPlayer::GetOutPutPath()
{
    if (m_Material == nullptr)
    {
        GJHGameEngineDebug::AssertMsg("if (nullptr == m_Material)");
    }

    return m_Material->GetPixelShader()->GetOutPutPathType();
}