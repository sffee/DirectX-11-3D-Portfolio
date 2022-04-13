#pragma once
#include "GJHResBase.h"
#include "GJHDirectResourcesManager.h"
#include "FW1FontWrapper.h"

#pragma comment(lib, "FW1FontWrapper.lib")

class GJHDirectFont : public GJHResBase, public GJHDirectResourcesManager<GJHDirectFont>
{
private:
	friend class GJHGameEngineDevice;

private:
	static IFW1Factory* m_pFontFactory;
	static void Init();
	static void Release();

public:
	static bool Create(const GJHGameEngineString& _Name)
	{
		GJHDirectFont* NewRes = new GJHDirectFont();
		if (NewRes->FontLoad(_Name) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

public:
	GJHDirectFont();
	~GJHDirectFont();

public:
	GJHDirectFont(const GJHDirectFont& _Other) = delete;
	GJHDirectFont(const GJHDirectFont&& _Other) = delete;

public:
	GJHDirectFont operator=(const GJHDirectFont& _Other) = delete;
	GJHDirectFont operator=(const GJHDirectFont&& _Other) = delete;

public:
	IFW1FontWrapper* m_pFontWrapper;

public:
	bool FontLoad(const GJHGameEngineString& _Name);
	void DrawFont(const GJHGameEngineString& _Text, float _Size, float4 _Pos, float4 _Color = float4::RED, FW1_TEXT_FLAG _Flag = FW1_TOP);
	void DrawFontGeo();
};