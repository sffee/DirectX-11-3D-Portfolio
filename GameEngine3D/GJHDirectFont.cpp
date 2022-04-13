#include "GJHDirectFont.h"
#include "GJHGameEngineDevice.h"

IFW1Factory* GJHDirectFont::m_pFontFactory = nullptr;

void GJHDirectFont::Init()
{
	if (FW1CreateFactory(FW1_VERSION, &m_pFontFactory) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (FW1CreateFactory(FW1_VERSION, &m_pFontFactory) != S_OK)");
	}

	GJHDirectFont::Create(L"±Ã¼­");
}

void GJHDirectFont::Release()
{
	if (m_pFontFactory != nullptr)
	{
		m_pFontFactory->Release();
		m_pFontFactory = nullptr;
	}
}

GJHDirectFont::GJHDirectFont()
{

}

GJHDirectFont::~GJHDirectFont()
{
	if (m_pFontWrapper != nullptr)
	{
		m_pFontWrapper->Release();
	}
}

bool GJHDirectFont::FontLoad(const GJHGameEngineString& _Name)
{
	if (m_pFontFactory->CreateFontWrapper(GJHGameEngineDevice::GetDevice(), _Name.c_str(), &m_pFontWrapper) != S_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (m_pFontFactory->CreateFontWrapper(GJHGameEngineDevice::GetDevice(), _Name.c_str(), &m_pFontWrapper) != S_OK)");
		return false;
	}

	return true;
}

void GJHDirectFont::DrawFont(const GJHGameEngineString& _Text, float _Size, float4 _Pos, float4 _Color, FW1_TEXT_FLAG _Flag)
{
	m_pFontWrapper->DrawString(GJHGameEngineDevice::GetContext(), _Text.c_str(), _Size, _Pos.x, _Pos.y, _Color.ColorToUint(), _Flag);
}

void GJHDirectFont::DrawFontGeo()
{

}