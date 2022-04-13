#include "GJHGridRenderer.h"
#include <GJHDirectSprite.h>
#include "GJHCamera.h"

GJHGridRenderer::GJHGridRenderer()
{
}

GJHGridRenderer::~GJHGridRenderer()
{
}

void GJHGridRenderer::Start(int _Order)
{
    GJHRenderer::Start(_Order);
    std::shared_ptr<GJHRenderPlayer> RP = CreateRenderPlayer(L"EngineGrid3D", L"GridRect");

    Data.Color = float4::WHITE;
    Data.LineData.x = 1.0f;

    RP->CBufferSettingLink(L"GRIDDATA", Data);
}

void GJHGridRenderer::SetTargetCamera(std::shared_ptr<GJHCamera> _Target)
{
    m_Target = _Target;
    SetWorldScale({ m_Target->GetFar(), m_Target->GetFar(), m_Target->GetFar(), 1.0f });
}


void GJHGridRenderer::Update()
{
    float4 Pos = m_Target->GetActorTrans()->GetWorldPosition();
    Data.CamPos = Pos;

    Data.LineData.x = Data.CamPos.y * 0.01f;
    Data.LineData.y = 1.0f;

    SetWorldPosition({ Pos.x, 0.0f, Pos.z, 1.0f });
}