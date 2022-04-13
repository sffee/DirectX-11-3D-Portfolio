#include "GJHHeightMapRenderer.h"
#include <GJHGameEngineRandom.h>

GJHHeightMapRenderer::GJHHeightMapRenderer()
{
    memset(Data.IsSplatting, 0, sizeof(Data.IsSplatting));
}

GJHHeightMapRenderer::~GJHHeightMapRenderer()
{
}

void GJHHeightMapRenderer::CreateMap(int _X, int _Z, float YRatio, const GJHGameEngineString& _HTexName)
{
    HTex = GJHDirectTexture2D::Find(_HTexName);

    if (HTex == nullptr)
    {
        GJHGameEngineDebug::AssertMsg("HTex = DirectTexture2D::Load(_HTexPath)");
    }

    {
        Data.MapCount.x = static_cast<float>(_X);
        Data.MapCount.y = YRatio;
        Data.MapCount.z = static_cast<float>(_Z);

        std::shared_ptr<GJHDirectVertexBuffer> VTXBuffer = std::make_shared<GJHDirectVertexBuffer>();

        VBVector.reserve(((size_t)_X + 1) * ((size_t)_Z + 1));

        VertexBase CurPos;

        float4 HTexSize = HTex->GetSize();

        for (int z = 0; z < _Z + 1; z++)
        {
            for (int x = 0; x < _X + 1; x++)
            {
                CurPos.Pos.x = static_cast<float>(x);
                CurPos.Pos.y = 0.f;
                CurPos.Pos.z = static_cast<float>(-z);

                CurPos.Uv.x = static_cast<float>(x);
                CurPos.Uv.z = static_cast<float>(z);
                CurPos.Normal = float4::UP;
                VBVector.push_back(CurPos);

                float4 Color = HTex->GetPixel(static_cast<int>(x * HTexSize.x / _X), static_cast<int>(z * HTexSize.y / _Z));
                VBVectorPos.push_back(float4(static_cast<float>(x), Color.y * Data.MapCount.y, static_cast<float>(-z)));
            }
        }

        VTXBuffer->Create(&VBVector[0], sizeof(VertexBase), VBVector.size(), D3D11_USAGE::D3D11_USAGE_DYNAMIC);

        IBVector.reserve(((size_t)_X * _Z) * 6);

        std::shared_ptr<GJHDirectIndexBuffer> IDXBuffer = std::make_shared<GJHDirectIndexBuffer>();

        for (int z = 0; z < _Z; z++)
        {
            for (int x = 0; x < _X; x++)
            {
                int PivotIndex = z * (_X + 1) + x;

                IBVector.push_back(PivotIndex);
                IBVector.push_back(PivotIndex + 1);
                IBVector.push_back(PivotIndex + (_X + 1) + 1);

                IBVector.push_back(PivotIndex);
                IBVector.push_back(PivotIndex + (_X + 1) + 1);
                IBVector.push_back(PivotIndex + (_X + 1));
            }
        }

        IDXBuffer->Create(&IBVector[0], sizeof(UINT), IBVector.size(), DXGI_FORMAT_R32_UINT, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

        m_HeightMesh = std::make_shared<GJHDirectMesh>();
        m_HeightMesh->IndexBufferPush(IDXBuffer);
        m_HeightMesh->VertexBufferPush(VTXBuffer);

        if (IBVector.size() % 3 != 0)
        {
            GJHGameEngineDebug::AssertMsg("if (IBVector.size() % 3 != 0)");
        }
    }

    std::shared_ptr<GJHRenderPlayer> Player = CreateRenderPlayer(L"DefferdHeightMap", m_HeightMesh);
    Player->CBufferSettingLink(L"MapData", Data);
    Player->TextureSetting(L"HTex", HTex);
}

void GJHHeightMapRenderer::SetFloor(int _FloorCount, const GJHGameEngineString& _DiffuseTexture, const GJHGameEngineString& _SplettingTexture)
{
    Data.IsSplatting[_FloorCount] = 1;

    GJHGameEngineString DiffuseSettingName = L"SDif";
    DiffuseSettingName += _FloorCount;

    if (_FloorCount == 0)
    {
        DiffuseSettingName = L"BaseTex";
    }

    TextureSetting(DiffuseSettingName, _DiffuseTexture);

    if (_FloorCount != 0)
    {
        GJHGameEngineString SplettingSettingName = L"STex";
        SplettingSettingName += _FloorCount;
        TextureSetting(SplettingSettingName, _SplettingTexture);
    }
}