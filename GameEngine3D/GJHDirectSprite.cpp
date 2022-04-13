#include "GJHDirectSprite.h"
#include "GJHDirectTexture2D.h"
#include <GJHGameEngineDebug.h>

GJHDirectSprite::GJHDirectSprite()
{

}

GJHDirectSprite::~GJHDirectSprite()
{

}

void GJHDirectSprite::CreateRes(const GJHGameEngineString& _TexName, float4 _CutCount)
{
	m_Tex = GJHDirectTexture2D::Find(_TexName);

	if (m_Tex == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Tex == nullptr)");
	}

	int X = _CutCount.ix();
	int Y = _CutCount.iy();

	float4 CurData = { 0.f, 0.f };
	CurData.z = 1 / _CutCount.x;
	CurData.w = 1 / _CutCount.y;

	m_CutDataSize.x = _CutCount.x;
	m_CutDataSize.y = _CutCount.y;
	m_CutDataSize.z = (float)(m_Tex->GetSizeX() / X);
	m_CutDataSize.w = (float)(m_Tex->GetSizeY() / Y);

	for (size_t y = 0; y < Y; y++)
	{
		for (size_t x = 0; x < X; x++)
		{
			CurData.x = CurData.z * x;
			m_CutData.push_back(CurData);
		}

		CurData.x = 0.f;
		CurData.y += CurData.w;
	}
}

void GJHDirectSprite::CreateResPiece(const GJHGameEngineString& _TexName)
{
	m_Tex = GJHDirectTexture2D::Find(_TexName);

	if (m_Tex == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Tex == nullptr)");
	}

	m_TexPiece.push_back(m_Tex);
}